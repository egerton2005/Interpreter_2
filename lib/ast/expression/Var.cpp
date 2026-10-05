#include "Var.hpp"

#include "visitor/IExprVisitor.hpp"

Var::Var(std::string name)
: name_(name)
{ }

const std::string& Var::GetName() const noexcept {
    return name_;
}

void Var::Accept(IExprVisitor& visitor) const {
    visitor.Visit(*this);
}