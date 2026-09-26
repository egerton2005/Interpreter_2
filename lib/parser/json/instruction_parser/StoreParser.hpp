#pragma once

#include "parser/json/instruction_parser/IJsonInstructionParser.hpp"

class StoreParser final : public IJsonInstructionParser {
public:
    InstructionPtr Parse(const nlohmann::json& node) const override;
};
