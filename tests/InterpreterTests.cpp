#include <memory>
#include <sstream>
#include <string>

#include <gtest/gtest.h>

#include "binary_operator/BinaryOperatorRegistry.hpp"
#include "error/Errors.hpp"
#include "instruction/Instruction.hpp"
#include "parser/json/JsonParser.hpp"
#include "parser/json/instruction_parser/BinaryOperationParser.hpp"
#include "parser/json/instruction_parser/ConstantParser.hpp"
#include "parser/json/instruction_parser/JumpNotZeroParser.hpp"
#include "parser/json/instruction_parser/JumpParser.hpp"
#include "parser/json/instruction_parser/JumpZeroParser.hpp"
#include "parser/json/instruction_parser/LabelParser.hpp"
#include "parser/json/instruction_parser/LoadParser.hpp"
#include "parser/json/instruction_parser/ReadParser.hpp"
#include "parser/json/instruction_parser/StoreParser.hpp"
#include "parser/json/instruction_parser/WriteParser.hpp"
#include "runtime/Interpreter.hpp"
#include "runtime/Machine.hpp"
#include "runtime/io/StreamInput.hpp"
#include "runtime/io/StreamOutput.hpp"

namespace {

JsonInstructionParserRegistry CreateInstructionParsers() {
    JsonInstructionParserRegistry::Builder builder;
    builder.Add("READ", std::make_shared<ReadParser>())
           .Add("WRITE", std::make_shared<WriteParser>())
           .Add("LD", std::make_shared<LoadParser>())
           .Add("ST", std::make_shared<StoreParser>())
           .Add("CONST", std::make_shared<ConstantParser>())
           .Add(
               "BINOP",
               std::make_shared<BinaryOperationParser>(CreateDefaultBinaryOperators())
           )
           .Add("LABEL", std::make_shared<LabelParser>())
           .Add("JMP", std::make_shared<JumpParser>())
           .Add("JZ", std::make_shared<JumpZeroParser>())
           .Add("JNZ", std::make_shared<JumpNotZeroParser>());
    return builder.Build();
}

std::string RunProgram(const std::string& source, const std::string& input = "") {
    std::istringstream program(source);
    std::istringstream runtime_input(input);
    std::ostringstream runtime_output;

    auto parser = std::make_shared<JsonParser>(CreateInstructionParsers());
    auto machine = std::make_shared<Machine>(
        std::make_shared<StreamInput>(runtime_input),
        std::make_shared<StreamOutput>(runtime_output)
    );
    Interpreter interpreter(std::move(parser), std::move(machine));
    interpreter.Run(program);

    return runtime_output.str();
}

} // namespace

TEST(JsonParserTest, ParsesEveryInstructionKind) {
    std::istringstream source(R"([
        "READ",
        "WRITE",
        {"LD":"x"},
        {"ST":"x"},
        {"CONST":1},
        {"BINOP":"+"},
        {"LABEL":"start"},
        {"JMP":"start"},
        {"JZ":"start"},
        {"JNZ":"start"}
    ])");
    JsonParser parser(CreateInstructionParsers());

    const Program program = parser.Parse(source);

    ASSERT_EQ(program.size(), 10U);
    EXPECT_NE(std::dynamic_pointer_cast<const ReadInstruction>(program[0]), nullptr);
    EXPECT_NE(std::dynamic_pointer_cast<const WriteInstruction>(program[1]), nullptr);
    EXPECT_NE(std::dynamic_pointer_cast<const LoadInstruction>(program[2]), nullptr);
    EXPECT_NE(std::dynamic_pointer_cast<const StoreInstruction>(program[3]), nullptr);
    EXPECT_NE(std::dynamic_pointer_cast<const ConstantInstruction>(program[4]), nullptr);
    EXPECT_NE(std::dynamic_pointer_cast<const BinaryOperationInstruction>(program[5]), nullptr);
    EXPECT_NE(std::dynamic_pointer_cast<const LabelInstruction>(program[6]), nullptr);
    EXPECT_NE(std::dynamic_pointer_cast<const JumpInstruction>(program[7]), nullptr);
    EXPECT_NE(std::dynamic_pointer_cast<const JumpZeroInstruction>(program[8]), nullptr);
    EXPECT_NE(std::dynamic_pointer_cast<const JumpNotZeroInstruction>(program[9]), nullptr);
}

TEST(JsonParserTest, RejectsNonArrayProgram) {
    EXPECT_THROW(RunProgram(R"({"CONST":1})"), SyntaxError);
}

TEST(JsonParserTest, RejectsMalformedInstructionObject) {
    EXPECT_THROW(RunProgram(R"([{"CONST":1,"LD":"x"}])"), SyntaxError);
}

TEST(JsonParserTest, RejectsUnknownInstruction) {
    EXPECT_THROW(RunProgram(R"(["HALT"])"), SyntaxError);
}

TEST(JsonParserTest, RejectsInvalidOperandType) {
    EXPECT_THROW(RunProgram(R"([{"LD":42}])"), SyntaxError);
    EXPECT_THROW(RunProgram(R"([{"CONST":"42"}])"), SyntaxError);
}

TEST(JsonParserTest, RejectsUnknownBinaryOperator) {
    EXPECT_THROW(RunProgram(R"([{"BINOP":"^"}])"), SyntaxError);
}

TEST(MachineTest, PreservesOperandOrder) {
    EXPECT_EQ(RunProgram(R"([
        {"CONST":7}, {"CONST":3}, {"BINOP":"-"}, "WRITE"
    ])"), "4\n");
}

TEST(MachineTest, StoresAndLoadsVariables) {
    EXPECT_EQ(RunProgram(R"([
        {"CONST":21}, {"ST":"x"}, {"LD":"x"}, {"CONST":2},
        {"BINOP":"*"}, "WRITE"
    ])"), "42\n");
}

TEST(MachineTest, ReadsAndWritesValues) {
    EXPECT_EQ(RunProgram(R"(["READ", "WRITE"])", "42"), "42\n");
}

TEST(MachineTest, PerformsUnconditionalJump) {
    EXPECT_EQ(RunProgram(R"([
        {"JMP":"target"}, {"CONST":1}, "WRITE",
        {"LABEL":"target"}, {"CONST":2}, "WRITE"
    ])"), "2\n");
}

TEST(MachineTest, JumpsOnZero) {
    EXPECT_EQ(RunProgram(R"([
        {"CONST":0}, {"JZ":"zero"}, {"CONST":1}, "WRITE", {"JMP":"end"},
        {"LABEL":"zero"}, {"CONST":2}, "WRITE", {"LABEL":"end"}
    ])"), "2\n");
}

TEST(MachineTest, JumpsOnNonZero) {
    EXPECT_EQ(RunProgram(R"([
        {"CONST":5}, {"JNZ":"nonzero"}, {"CONST":1}, "WRITE", {"JMP":"end"},
        {"LABEL":"nonzero"}, {"CONST":2}, "WRITE", {"LABEL":"end"}
    ])"), "2\n");
}

TEST(MachineTest, EvaluatesLogicalOperators) {
    EXPECT_EQ(RunProgram(R"([
        {"CONST":0}, {"CONST":7}, {"BINOP":"!!"}, "WRITE",
        {"CONST":1}, {"CONST":0}, {"BINOP":"&&"}, "WRITE"
    ])"), "1\n0\n");
}

TEST(MachineTest, RejectsStackUnderflow) {
    EXPECT_THROW(RunProgram(R"(["WRITE"])"), StackUnderflow);
    EXPECT_THROW(RunProgram(R"([{"CONST":1},{"BINOP":"+"}])"), StackUnderflow);
}

TEST(MachineTest, RejectsUndefinedVariable) {
    EXPECT_THROW(RunProgram(R"([{"LD":"missing"}])"), UndefinedVariable);
}

TEST(MachineTest, RejectsUndefinedAndDuplicateLabels) {
    EXPECT_THROW(RunProgram(R"([{"JMP":"missing"}])"), UndefinedLabel);
    EXPECT_THROW(RunProgram(R"([
        {"LABEL":"same"}, {"LABEL":"same"}
    ])"), DuplicateLabel);
}

TEST(MachineTest, RejectsDivisionByZero) {
    EXPECT_THROW(RunProgram(R"([
        {"CONST":10}, {"CONST":0}, {"BINOP":"/"}
    ])"), DivisionByZero);
}

TEST(InterpreterTest, CalculatesFactorial) {
    EXPECT_EQ(RunProgram(R"([
        "READ", {"ST":"n"}, {"CONST":1}, {"ST":"result"},
        {"LABEL":"loop"},
        {"LD":"n"}, {"CONST":1}, {"BINOP":">"}, {"JZ":"done"},
        {"LD":"result"}, {"LD":"n"}, {"BINOP":"*"}, {"ST":"result"},
        {"LD":"n"}, {"CONST":1}, {"BINOP":"-"}, {"ST":"n"},
        {"JMP":"loop"},
        {"LABEL":"done"}, {"LD":"result"}, "WRITE"
    ])", "5"), "120\n");
}

TEST(InterpreterTest, RunsFactorialProducedByReferenceCompiler) {
    EXPECT_EQ(RunProgram(R"([
        "READ",
        {"ST":"n"},
        {"CONST":1},
        {"ST":"result"},
        {"JMP":"L_while_cond_5"},
        {"LABEL":"L_while_cody_4"},
        {"LD":"result"},
        {"LD":"n"},
        {"BINOP":"*"},
        {"ST":"result"},
        {"LD":"n"},
        {"CONST":1},
        {"BINOP":"-"},
        {"ST":"n"},
        {"LABEL":"L_while_cond_5"},
        {"LD":"n"},
        {"CONST":1},
        {"BINOP":">"},
        {"JNZ":"L_while_cody_4"},
        {"LD":"result"},
        "WRITE"
    ])", "10"), "3628800\n");
}
