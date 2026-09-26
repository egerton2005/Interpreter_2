#pragma once

#include "binary_operator/BinaryOperatorRegistry.hpp"
#include "parser/json/instruction_parser/IJsonInstructionParser.hpp"

class BinaryOperationParser final : public IJsonInstructionParser {
public:
    explicit BinaryOperationParser(BinaryOperatorRegistryPtr operators);
    InstructionPtr Parse(const nlohmann::json& node) const override;

private:
    BinaryOperatorRegistryPtr operators_;
};
