#include "parser/json/instruction_parser/JsonInstructionParserRegistry.hpp"

#include <string>
#include <utility>

#include "error/Errors.hpp"

JsonInstructionParserRegistry::Builder& JsonInstructionParserRegistry::Builder::Add(
    std::string key,
    JsonInstructionParserPtr parser
) {
    const std::string saved_key = key;
    const bool inserted = parsers_.emplace(std::move(key), std::move(parser)).second;
    if (!inserted) {
        throw RuntimeError("duplicate instruction parser: " + saved_key);
    }
    return *this;
}

JsonInstructionParserRegistry JsonInstructionParserRegistry::Builder::Build() {
    return JsonInstructionParserRegistry(std::move(parsers_));
}

InstructionPtr JsonInstructionParserRegistry::Parse(const nlohmann::json& node) const {
    std::string key;
    if (node.is_string()) {
        key = node.get<std::string>();
    } else if (node.is_object() && node.size() == 1) {
        key = node.begin().key();
    } else {
        throw SyntaxError(
            "instruction must be READ, WRITE, or a single-field object: " + node.dump()
        );
    }

    const auto parser = parsers_.find(key);
    if (parser == parsers_.end()) {
        throw SyntaxError("unknown instruction: " + key);
    }
    return parser->second->Parse(node);
}

JsonInstructionParserRegistry::JsonInstructionParserRegistry(
    std::unordered_map<std::string, JsonInstructionParserPtr> parsers
)
: parsers_(std::move(parsers))
{ }
