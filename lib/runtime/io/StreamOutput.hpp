#pragma once

#include <ostream>

#include "runtime/io/IOutput.hpp"

class StreamOutput final : public IOutput {
public:
    explicit StreamOutput(std::ostream& output);
    void Write(const Value& value) override;

private:
    std::ostream& output_;
};
