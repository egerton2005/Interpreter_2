#include "parser/json/instruction_parser/WriteParser.hpp"

#include "parser/json/JsonUtils.hpp"

InstructionPtr WriteParser::Parse(const nlohmann::json& node) const {
    ExpectKeyword(node, "WRITE");
    return std::make_shared<WriteInstruction>();
}
