#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "binary_operator/IBinaryOperator.hpp"
#include "value/Value.hpp"

class ReadInstruction;
class WriteInstruction;
class LoadInstruction;
class StoreInstruction;
class ConstantInstruction;
class BinaryOperationInstruction;
class LabelInstruction;
class JumpInstruction;
class JumpZeroInstruction;
class JumpNotZeroInstruction;

class IInstructionVisitor {
public:
    virtual void Visit(const ReadInstruction& instruction) = 0;
    virtual void Visit(const WriteInstruction& instruction) = 0;
    virtual void Visit(const LoadInstruction& instruction) = 0;
    virtual void Visit(const StoreInstruction& instruction) = 0;
    virtual void Visit(const ConstantInstruction& instruction) = 0;
    virtual void Visit(const BinaryOperationInstruction& instruction) = 0;
    virtual void Visit(const LabelInstruction& instruction) = 0;
    virtual void Visit(const JumpInstruction& instruction) = 0;
    virtual void Visit(const JumpZeroInstruction& instruction) = 0;
    virtual void Visit(const JumpNotZeroInstruction& instruction) = 0;
    virtual ~IInstructionVisitor() = default;
};

class IInstruction {
public:
    virtual void Accept(IInstructionVisitor& visitor) const = 0;
    virtual ~IInstruction() = default;
};

using InstructionPtr = std::shared_ptr<const IInstruction>;
using Program = std::vector<InstructionPtr>;

class ReadInstruction final : public IInstruction {
public:
    void Accept(IInstructionVisitor& visitor) const override;
};

class WriteInstruction final : public IInstruction {
public:
    void Accept(IInstructionVisitor& visitor) const override;
};

class LoadInstruction final : public IInstruction {
public:
    explicit LoadInstruction(std::string name);
    const std::string& GetName() const noexcept;
    void Accept(IInstructionVisitor& visitor) const override;

private:
    std::string name_;
};

class StoreInstruction final : public IInstruction {
public:
    explicit StoreInstruction(std::string name);
    const std::string& GetName() const noexcept;
    void Accept(IInstructionVisitor& visitor) const override;

private:
    std::string name_;
};

class ConstantInstruction final : public IInstruction {
public:
    explicit ConstantInstruction(Value value);
    const Value& GetValue() const noexcept;
    void Accept(IInstructionVisitor& visitor) const override;

private:
    Value value_;
};

class BinaryOperationInstruction final : public IInstruction {
public:
    explicit BinaryOperationInstruction(IBinaryOperatorPtr op);
    IBinaryOperatorPtr GetOperator() const noexcept;
    void Accept(IInstructionVisitor& visitor) const override;

private:
    IBinaryOperatorPtr operator_;
};

class LabelInstruction final : public IInstruction {
public:
    explicit LabelInstruction(std::string name);
    const std::string& GetName() const noexcept;
    void Accept(IInstructionVisitor& visitor) const override;

private:
    std::string name_;
};

class JumpInstruction final : public IInstruction {
public:
    explicit JumpInstruction(std::string label);
    const std::string& GetLabel() const noexcept;
    void Accept(IInstructionVisitor& visitor) const override;

private:
    std::string label_;
};

class JumpZeroInstruction final : public IInstruction {
public:
    explicit JumpZeroInstruction(std::string label);
    const std::string& GetLabel() const noexcept;
    void Accept(IInstructionVisitor& visitor) const override;

private:
    std::string label_;
};

class JumpNotZeroInstruction final : public IInstruction {
public:
    explicit JumpNotZeroInstruction(std::string label);
    const std::string& GetLabel() const noexcept;
    void Accept(IInstructionVisitor& visitor) const override;

private:
    std::string label_;
};
