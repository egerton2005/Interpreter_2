#include "Assign.hpp"

#include <utility>

#include "ast/statement/visitor/IStmtVisitor.hpp"
#include "error/Errors.hpp"

Assign::Assign(std::string name, IExprPtr value)
: name_(std::move(name))
, value_(std::move(value))
{ }

const std::string& Assign::GetName() const noexcept {
    return name_;
}

IExprPtr Assign::GetValue() const noexcept {
    return value_;
}

void Assign::Accept(IStmtVisitor& visitor) const {
    visitor.Visit(*this);
}