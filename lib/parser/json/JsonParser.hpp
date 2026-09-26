#pragma once

#include "parser/IParser.hpp"
#include "parser/json/instruction_parser/JsonInstructionParserRegistry.hpp"

class JsonParser final : public IParser {
public:
    explicit JsonParser(JsonInstructionParserRegistry instructions);

    Program Parse(std::istream& input) const override;
    InstructionPtr ParseInstruction(const nlohmann::json& node) const;

private:
    JsonInstructionParserRegistry instructions_;
};
