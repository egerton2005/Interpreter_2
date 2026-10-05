#include "If.hpp"

#include "ast/statement/visitor/IStmtVisitor.hpp"
#include "error/Errors.hpp"

If::If(IExprPtr cond, IStmtPtr thenBranch, IStmtPtr elseBranch)
: cond_(std::move(cond))
, then_(std::move(thenBranch))
, else_(std::move(elseBranch))
{ }

IExprPtr If::GetCond() const noexcept {
    return cond_;
}

IStmtPtr If::GetThenBranch() const noexcept {
    return then_;
}

IStmtPtr If::GetElseBranch() const noexcept {
    return else_;
}

void If::Accept(IStmtVisitor& visitor) const {
    visitor.Visit(*this);
}