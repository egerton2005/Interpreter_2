#include "parser/json/instruction_parser/LabelParser.hpp"

#include "parser/json/JsonUtils.hpp"

InstructionPtr LabelParser::Parse(const nlohmann::json& node) const {
    return std::make_shared<LabelInstruction>(GetStringOperand(node, "LABEL"));
}
