#pragma once

#include <memory>

#include "value/Value.hpp"

class IInput {
public:
    virtual Value Read() = 0;
    virtual ~IInput() = default;
};

using InputPtr = std::shared_ptr<IInput>;
