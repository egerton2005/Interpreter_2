#pragma once

#include <memory>

#include "instruction/Instruction.hpp"

class IMachine {
public:
    virtual void Execute(const Program& program) = 0;
    virtual ~IMachine() = default;
};

using MachinePtr = std::shared_ptr<IMachine>;
