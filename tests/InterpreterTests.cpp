#include <memory>
#include <sstream>
#include <string>

#include <gtest/gtest.h>

#include "binary_operator/Add.hpp"
#include "binary_operator/And.hpp"
#include "binary_operator/BinaryOperatorRegistry.hpp"
#include "binary_operator/Div.hpp"
#include "binary_operator/Eq.hpp"
#include "binary_operator/Ge.hpp"
#include "binary_operator/Gt.hpp"
#include "binary_operator/Le.hpp"
#include "binary_operator/Lt.hpp"
#include "binary_operator/Mod.hpp"
#include "binary_operator/Mul.hpp"
#include "binary_operator/Ne.hpp"
#include "binary_operator/Or.hpp"
#include "binary_operator/Sub.hpp"
#include "compiler/Compiler.hpp"
#include "error/Errors.hpp"
#include "instruction/Instruction.hpp"
#include "lexer/Lexer.hpp"
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
#include "parser/text/Parser.hpp"
#include "runtime/Interpreter.hpp"
#include "runtime/Machine.hpp"
#include "runtime/io/StreamInput.hpp"
#include "runtime/io/StreamOutput.hpp"
#include "parser/text/statement_parser/StatementParsers.hpp"

BinaryOperatorRegistryPtr CreateDefaultBinaryOperators() {
	BinaryOperatorRegistry::Builder builder;
	builder.Add("+", std::make_shared<Add>())
		.Add("-", std::make_shared<Sub>())
		.Add("*", std::make_shared<Mul>())
		.Add("/", std::make_shared<Div>())
		.Add("%", std::make_shared<Mod>())
		.Add("<", std::make_shared<Lt>())
		.Add("<=", std::make_shared<Le>())
		.Add(">", std::make_shared<Gt>())
		.Add(">=", std::make_shared<Ge>())
		.Add("==", std::make_shared<Eq>())
		.Add("!=", std::make_shared<Ne>())
		.Add("&&", std::make_shared<And>())
		.Add("!!", std::make_shared<Or>());
	return std::make_shared<BinaryOperatorRegistry>(builder.Build());
}

JsonInstructionParserRegistry CreateInstructionParsers() {
	JsonInstructionParserRegistry::Builder builder;
	builder.Add("READ", std::make_shared<ReadParser>())
		.Add("WRITE", std::make_shared<WriteParser>())
		.Add("LD", std::make_shared<LoadParser>())
		.Add("ST", std::make_shared<StoreParser>())
		.Add("CONST", std::make_shared<ConstantParser>())
		.Add("BINOP", std::make_shared<BinaryOperationParser>(CreateDefaultBinaryOperators()))
		.Add("LABEL", std::make_shared<LabelParser>())
		.Add("JMP", std::make_shared<JumpParser>())
		.Add("JZ", std::make_shared<JumpZeroParser>())
		.Add("JNZ", std::make_shared<JumpNotZeroParser>());
	return builder.Build();
}

StatementParserRegistry CreateStatementParsers() {
	StatementParserRegistry::Builder builder;
	builder.Add(Token::Skip, std::make_shared<SkipParser>())
		   .Add(Token::LBrace, std::make_shared<BlockParser>())
		   .Add(Token::Ident, std::make_shared<AssignParser>())
		   .Add(Token::Read, std::make_shared<ReadStatementParser>())
		   .Add(Token::Write, std::make_shared<WriteStatementParser>())
		   .Add(Token::If, std::make_shared<IfParser>())
		   .Add(Token::While, std::make_shared<WhileParser>())
		   .Add(Token::Do, std::make_shared<DoWhileParser>())
		   .Add(Token::For, std::make_shared<ForParser>());
	return builder.Build();
}

std::string Execute(const Program& program, const std::string& input) {
	std::istringstream runtime_input(input);
	std::ostringstream runtime_output;
	Machine machine(std::make_shared<StreamInput>(runtime_input), std::make_shared<StreamOutput>(runtime_output));
	machine.Execute(program);
	return runtime_output.str();
}

std::string RunProgram(const std::string& source, const std::string& input = "") {
	std::istringstream program(source);
	JsonParser parser(CreateInstructionParsers());
	return Execute(parser.Parse(program), input);
}

std::string RunSource(const std::string& source, const std::string& input = "") {
	Parser parser(Lexer(source), CreateStatementParsers(), CreateDefaultBinaryOperators());
	Compiler compiler;
	return Execute(compiler.Compile(*parser.ParseProgram()), input);
}


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

TEST(CompilerTest, CompilesFactorial) {
	EXPECT_EQ(RunSource(R"(
		read(n);
		result = 1;
		while (n > 0) {
			result *= n;
			n -= 1
		}
		write(result)
	)", "5"), "120\n");
}

TEST(CompilerTest, RespectsOperatorPrecedence) {
	EXPECT_EQ(RunSource("write(1 + 2 * 3)"), "7\n");
	EXPECT_EQ(RunSource("write((1 + 2) * 3)"), "9\n");
	EXPECT_EQ(RunSource("write(1 < 2 && 2 < 3)"), "1\n");
	EXPECT_EQ(RunSource("write(0 !! 1 && 0)"), "0\n");
}

TEST(CompilerTest, OperatorsAreLeftAssociative) {
	EXPECT_EQ(RunSource("write(10 - 3 - 2)"), "5\n");
	EXPECT_EQ(RunSource("write(100 / 10 / 2)"), "5\n");
}

TEST(CompilerTest, BindsElseToNearestIf) {
	EXPECT_EQ(RunSource("if (1) if (0) write(1) else write(2)"), "2\n");
	EXPECT_EQ(RunSource("if (0) if (1) write(1) else write(2)"), "");
}

TEST(CompilerTest, HandlesElifChain) {
	const std::string source = R"(
		read(x);
		if (x == 1) write(10)
		elif (x == 2) write(20)
		elif (x == 3) write(30)
		else write(0)
	)";
	EXPECT_EQ(RunSource(source, "1"), "10\n");
	EXPECT_EQ(RunSource(source, "2"), "20\n");
	EXPECT_EQ(RunSource(source, "3"), "30\n");
	EXPECT_EQ(RunSource(source, "9"), "0\n");
}

TEST(CompilerTest, HandlesForAndDoWhile) {
	EXPECT_EQ(RunSource(R"(
		s = 0;
		for (i = 1; i <= 4; i += 1) s += i;
		write(s)
	)"), "10\n");
	EXPECT_EQ(RunSource(R"(
		i = 0;
		do { write(i); i += 1 } while (i < 3)
	)"), "0\n1\n2\n");
}

TEST(CompilerTest, SkipsComments) {
	EXPECT_EQ(RunSource(R"(
		-- line comment
		x = 1; (* block
		comment *) write(x)
	)"), "1\n");
}

TEST(CompilerTest, ReportsSyntaxErrorsWithPosition) {
	try {
		RunSource("x = 1;\n  y = (2 + 3");
		FAIL();
	} catch (const ParseError& error) {
		EXPECT_EQ(std::string(error.what()), "2:13: expected ')', got ''");
	}
	EXPECT_THROW(RunSource("X = 1"), ParseError);
	EXPECT_THROW(RunSource("write(1 +)"), ParseError);
}