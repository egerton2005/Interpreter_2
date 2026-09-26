#pragma once

#include <istream>

#include "runtime/io/IInput.hpp"

class StreamInput final : public IInput {
public:
    explicit StreamInput(std::istream& input);
    Value Read() override;

private:
    std::istream& input_;
};
