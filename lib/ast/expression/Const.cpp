#include "Const.hpp"

#include "visitor/IExprVisitor.hpp"

Const::Const(const Value& value)
: value_(value)
{ }

const Value& Const::GetValue() const noexcept {
    return value_;
}

void Const::Accept(IExprVisitor& visitor) const {
    visitor.Visit(*this);
}