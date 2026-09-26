#include "runtime/Machine.hpp"

#include <memory>
#include <utility>

#include "error/Errors.hpp"

Machine::Machine(InputPtr input, OutputPtr output)
: input_(std::move(input))
, output_(std::move(output))
{ }

void Machine::Execute(const Program& program) {
    environment_.Clear();
    stack_.clear();
    BuildLabelTable(program);
    program_counter_ = 0;

    while (program_counter_ < program.size()) {
        const auto instruction = program[program_counter_++];
        instruction->Accept(*this);
    }
}

void Machine::Visit(const ReadInstruction&) {
    Push(input_->Read());
}

void Machine::Visit(const WriteInstruction&) {
    output_->Write(Pop());
}

void Machine::Visit(const LoadInstruction& instruction) {
    Push(environment_.Get(instruction.GetName()));
}

void Machine::Visit(const StoreInstruction& instruction) {
    environment_.Set(instruction.GetName(), Pop());
}

void Machine::Visit(const ConstantInstruction& instruction) {
    Push(instruction.GetValue());
}

void Machine::Visit(const BinaryOperationInstruction& instruction) {
    const Value rhs = Pop();
    const Value lhs = Pop();
    Push(instruction.GetOperator()->Apply(lhs, rhs));
}

void Machine::Visit(const LabelInstruction&) {
}

void Machine::Visit(const JumpInstruction& instruction) {
    program_counter_ = ResolveLabel(instruction.GetLabel());
}

void Machine::Visit(const JumpZeroInstruction& instruction) {
    if (!static_cast<bool>(Pop())) {
        program_counter_ = ResolveLabel(instruction.GetLabel());
    }
}

void Machine::Visit(const JumpNotZeroInstruction& instruction) {
    if (static_cast<bool>(Pop())) {
        program_counter_ = ResolveLabel(instruction.GetLabel());
    }
}

void Machine::BuildLabelTable(const Program& program) {
    labels_.clear();
    for (std::size_t index = 0; index < program.size(); ++index) {
        const auto label = std::dynamic_pointer_cast<const LabelInstruction>(program[index]);
        if (!label) {
            continue;
        }

        const bool inserted = labels_.emplace(label->GetName(), index).second;
        if (!inserted) {
            throw DuplicateLabel(label->GetName());
        }
    }
}

std::size_t Machine::ResolveLabel(std::string_view label) const {
    const auto iterator = labels_.find(std::string(label));
    if (iterator == labels_.end()) {
        throw UndefinedLabel(std::string(label));
    }
    return iterator->second;
}

void Machine::Push(Value value) {
    stack_.push_back(value);
}

Value Machine::Pop() {
    if (stack_.empty()) {
        throw StackUnderflow();
    }

    const Value result = stack_.back();
    stack_.pop_back();
    return result;
}
