#include "parser/json/instruction_parser/JumpNotZeroParser.hpp"

#include "parser/json/JsonUtils.hpp"

InstructionPtr JumpNotZeroParser::Parse(const nlohmann::json& node) const {
    return std::make_shared<JumpNotZeroInstruction>(GetStringOperand(node, "JNZ"));
}
