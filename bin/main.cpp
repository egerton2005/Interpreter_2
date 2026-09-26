#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>

#include "binary_operator/BinaryOperatorRegistry.hpp"
#include "error/Errors.hpp"
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

JsonInstructionParserRegistry CreateInstructionParsers(
    BinaryOperatorRegistryPtr operators
) {
    JsonInstructionParserRegistry::Builder builder;
    builder.Add("READ", std::make_shared<ReadParser>())
           .Add("WRITE", std::make_shared<WriteParser>())
           .Add("LD", std::make_shared<LoadParser>())
           .Add("ST", std::make_shared<StoreParser>())
           .Add("CONST", std::make_shared<ConstantParser>())
           .Add("BINOP", std::make_shared<BinaryOperationParser>(std::move(operators)))
           .Add("LABEL", std::make_shared<LabelParser>())
           .Add("JMP", std::make_shared<JumpParser>())
           .Add("JZ", std::make_shared<JumpZeroParser>())
           .Add("JNZ", std::make_shared<JumpNotZeroParser>());
    return builder.Build();
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <program.json>\n";
        return EXIT_FAILURE;
    }

    std::ifstream program(argv[1]);
    if (!program) {
        std::cerr << "cannot open file: " << argv[1] << '\n';
        return EXIT_FAILURE;
    }

    try {
        auto parser = std::make_shared<JsonParser>(
            CreateInstructionParsers(CreateDefaultBinaryOperators())
        );
        auto machine = std::make_shared<Machine>(
            std::make_shared<StreamInput>(std::cin),
            std::make_shared<StreamOutput>(std::cout)
        );
        Interpreter interpreter(std::move(parser), std::move(machine));
        interpreter.Run(program);
    } catch (const InterpreterError& error) {
        std::cerr << "error: " << error.what() << '\n';
        return EXIT_FAILURE;
    } catch (const std::exception& error) {
        std::cerr << "internal error: " << error.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
