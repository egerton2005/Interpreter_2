#include "parser/json/JsonParser.hpp"

#include <istream>
#include <string>
#include <utility>

#include "error/Errors.hpp"

JsonParser::JsonParser(JsonInstructionParserRegistry instructions)
: instructions_(std::move(instructions))
{ }

Program JsonParser::Parse(std::istream& input) const {
    nlohmann::json document;
    try {
        input >> document;
    } catch (const nlohmann::json::parse_error& error) {
        throw SyntaxError(std::string("invalid JSON: ") + error.what());
    }

    if (!document.is_array()) {
        throw SyntaxError("program must be a JSON array");
    }

    Program program;
    program.reserve(document.size());
    for (const auto& node : document) {
        program.push_back(ParseInstruction(node));
    }
    return program;
}

InstructionPtr JsonParser::ParseInstruction(const nlohmann::json& node) const {
    return instructions_.Parse(node);
}
