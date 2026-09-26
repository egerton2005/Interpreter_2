#include "parser/json/instruction_parser/JumpParser.hpp"

#include "parser/json/JsonUtils.hpp"

InstructionPtr JumpParser::Parse(const nlohmann::json& node) const {
    return std::make_shared<JumpInstruction>(GetStringOperand(node, "JMP"));
}
