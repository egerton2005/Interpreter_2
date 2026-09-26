#pragma once

#include <memory>

#include "value/Value.hpp"

class IOutput {
public:
    virtual void Write(const Value& value) = 0;
    virtual ~IOutput() = default;
};

using OutputPtr = std::shared_ptr<IOutput>;
