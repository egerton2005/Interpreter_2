#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

#include "parser/json/instruction_parser/IJsonInstructionParser.hpp"

class JsonInstructionParserRegistry {
public:
    class Builder {
    public:
        Builder& Add(std::string key, JsonInstructionParserPtr parser);
        JsonInstructionParserRegistry Build();

    private:
        std::unordered_map<std::string, JsonInstructionParserPtr> parsers_;
    };

    InstructionPtr Parse(const nlohmann::json& node) const;

private:
    explicit JsonInstructionParserRegistry(
        std::unordered_map<std::string, JsonInstructionParserPtr> parsers
    );

    std::unordered_map<std::string, JsonInstructionParserPtr> parsers_;
};
