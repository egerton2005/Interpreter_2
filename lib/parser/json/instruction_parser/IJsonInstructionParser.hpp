#pragma once

#include <memory>

#include <nlohmann/json.hpp>

#include "instruction/Instruction.hpp"

class IJsonInstructionParser {
public:
    virtual InstructionPtr Parse(const nlohmann::json& node) const = 0;
    virtual ~IJsonInstructionParser() = default;
};

using JsonInstructionParserPtr = std::shared_ptr<const IJsonInstructionParser>;
