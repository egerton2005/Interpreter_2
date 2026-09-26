#include "parser/json/instruction_parser/JumpZeroParser.hpp"

#include "parser/json/JsonUtils.hpp"

InstructionPtr JumpZeroParser::Parse(const nlohmann::json& node) const {
    return std::make_shared<JumpZeroInstruction>(GetStringOperand(node, "JZ"));
}
