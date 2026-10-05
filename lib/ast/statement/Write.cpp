#include "Write.hpp"

#include <utility>

#include "ast/statement/visitor/IStmtVisitor.hpp"
#include "error/Errors.hpp"

Write::Write(IExprPtr expr) 
: expr_(std::move(expr)) 
{ }

IExprPtr Write::GetExpr() const noexcept {
    return expr_;
}

void Write::Accept(IStmtVisitor& visitor) const {
    visitor.Visit(*this);
}