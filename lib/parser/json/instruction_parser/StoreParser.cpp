#include "parser/json/instruction_parser/StoreParser.hpp"

#include "parser/json/JsonUtils.hpp"

InstructionPtr StoreParser::Parse(const nlohmann::json& node) const {
    return std::make_shared<StoreInstruction>(GetStringOperand(node, "ST"));
}
