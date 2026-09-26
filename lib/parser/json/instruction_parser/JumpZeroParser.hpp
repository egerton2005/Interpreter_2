#pragma once

#include "parser/json/instruction_parser/IJsonInstructionParser.hpp"

class JumpZeroParser final : public IJsonInstructionParser {
public:
    InstructionPtr Parse(const nlohmann::json& node) const override;
};
