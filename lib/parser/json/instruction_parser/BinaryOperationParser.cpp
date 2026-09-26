#include "parser/json/instruction_parser/BinaryOperationParser.hpp"

#include <utility>

#include "parser/json/JsonUtils.hpp"

BinaryOperationParser::BinaryOperationParser(BinaryOperatorRegistryPtr operators)
: operators_(std::move(operators))
{ }

InstructionPtr BinaryOperationParser::Parse(const nlohmann::json& node) const {
    return std::make_shared<BinaryOperationInstruction>(
        operators_->Get(GetStringOperand(node, "BINOP"))
    );
}
