#include "instruction/Instruction.hpp"

#include <utility>

void ReadInstruction::Accept(IInstructionVisitor& visitor) const {
    visitor.Visit(*this);
}

void WriteInstruction::Accept(IInstructionVisitor& visitor) const {
    visitor.Visit(*this);
}

LoadInstruction::LoadInstruction(std::string name)
: name_(std::move(name))
{ }

const std::string& LoadInstruction::GetName() const noexcept {
    return name_;
}

void LoadInstruction::Accept(IInstructionVisitor& visitor) const {
    visitor.Visit(*this);
}

StoreInstruction::StoreInstruction(std::string name)
: name_(std::move(name))
{ }

const std::string& StoreInstruction::GetName() const noexcept {
    return name_;
}

void StoreInstruction::Accept(IInstructionVisitor& visitor) const {
    visitor.Visit(*this);
}

ConstantInstruction::ConstantInstruction(Value value)
: value_(value)
{ }

const Value& ConstantInstruction::GetValue() const noexcept {
    return value_;
}

void ConstantInstruction::Accept(IInstructionVisitor& visitor) const {
    visitor.Visit(*this);
}

BinaryOperationInstruction::BinaryOperationInstruction(IBinaryOperatorPtr op)
: operator_(std::move(op))
{ }

IBinaryOperatorPtr BinaryOperationInstruction::GetOperator() const noexcept {
    return operator_;
}

void BinaryOperationInstruction::Accept(IInstructionVisitor& visitor) const {
    visitor.Visit(*this);
}

LabelInstruction::LabelInstruction(std::string name)
: name_(std::move(name))
{ }

const std::string& LabelInstruction::GetName() const noexcept {
    return name_;
}

void LabelInstruction::Accept(IInstructionVisitor& visitor) const {
    visitor.Visit(*this);
}

JumpInstruction::JumpInstruction(std::string label)
: label_(std::move(label))
{ }

const std::string& JumpInstruction::GetLabel() const noexcept {
    return label_;
}

void JumpInstruction::Accept(IInstructionVisitor& visitor) const {
    visitor.Visit(*this);
}

JumpZeroInstruction::JumpZeroInstruction(std::string label)
: label_(std::move(label))
{ }

const std::string& JumpZeroInstruction::GetLabel() const noexcept {
    return label_;
}

void JumpZeroInstruction::Accept(IInstructionVisitor& visitor) const {
    visitor.Visit(*this);
}

JumpNotZeroInstruction::JumpNotZeroInstruction(std::string label)
: label_(std::move(label))
{ }

const std::string& JumpNotZeroInstruction::GetLabel() const noexcept {
    return label_;
}

void JumpNotZeroInstruction::Accept(IInstructionVisitor& visitor) const {
    visitor.Visit(*this);
}
