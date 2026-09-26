#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "instruction/Instruction.hpp"
#include "runtime/IMachine.hpp"
#include "runtime/environment/Environment.hpp"
#include "runtime/io/IInput.hpp"
#include "runtime/io/IOutput.hpp"

class Machine final : public IMachine, public IInstructionVisitor {
public:
    Machine(InputPtr input, OutputPtr output);

    void Execute(const Program& program) override;

    void Visit(const ReadInstruction& instruction) override;
    void Visit(const WriteInstruction& instruction) override;
    void Visit(const LoadInstruction& instruction) override;
    void Visit(const StoreInstruction& instruction) override;
    void Visit(const ConstantInstruction& instruction) override;
    void Visit(const BinaryOperationInstruction& instruction) override;
    void Visit(const LabelInstruction& instruction) override;
    void Visit(const JumpInstruction& instruction) override;
    void Visit(const JumpZeroInstruction& instruction) override;
    void Visit(const JumpNotZeroInstruction& instruction) override;

private:
    void BuildLabelTable(const Program& program);
    std::size_t ResolveLabel(std::string_view label) const;
    void Push(Value value);
    Value Pop();

    InputPtr input_;
    OutputPtr output_;
    Environment environment_;
    std::vector<Value> stack_;
    std::unordered_map<std::string, std::size_t> labels_;
    std::size_t program_counter_ = 0;
};
