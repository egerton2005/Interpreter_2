#include "parser/json/instruction_parser/LoadParser.hpp"

#include "parser/json/JsonUtils.hpp"

InstructionPtr LoadParser::Parse(const nlohmann::json& node) const {
    return std::make_shared<LoadInstruction>(GetStringOperand(node, "LD"));
}
