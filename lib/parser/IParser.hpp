#pragma once

#include <iosfwd>
#include <memory>

#include "instruction/Instruction.hpp"

class IParser {
public:
    virtual Program Parse(std::istream& input) const = 0;
    virtual ~IParser() = default;
};

using ParserPtr = std::shared_ptr<const IParser>;
