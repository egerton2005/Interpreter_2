#include "parser/json/instruction_parser/ReadParser.hpp"

#include "parser/json/JsonUtils.hpp"

InstructionPtr ReadParser::Parse(const nlohmann::json& node) const {
    ExpectKeyword(node, "READ");
    return std::make_shared<ReadInstruction>();
}
