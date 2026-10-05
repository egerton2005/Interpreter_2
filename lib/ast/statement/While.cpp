#include "While.hpp"

#include "ast/statement/visitor/IStmtVisitor.hpp"
#include "error/Errors.hpp"

While::While(IExprPtr cond, IStmtPtr body)
: cond_(std::move(cond))
, body_(std::move(body))
{ }

IExprPtr While::GetCond() const noexcept {
    return cond_;
}

IStmtPtr While::GetBody() const noexcept {
    return body_;
}

void While::Accept(IStmtVisitor& visitor) const {
    visitor.Visit(*this);
}