#include "parser/json/instruction_parser/ConstantParser.hpp"

#include <cstdint>

#include "error/Errors.hpp"
#include "parser/json/JsonUtils.hpp"

InstructionPtr ConstantParser::Parse(const nlohmann::json& node) const {
    const auto& literal = GetInstructionOperand(node, "CONST");
    if (!literal.is_number_integer()) {
        throw SyntaxError("CONST operand must be an integer");
    }

    return std::make_shared<ConstantInstruction>(
        Value(literal.get<std::int32_t>())
    );
}
