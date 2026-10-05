#include "DoWhile.hpp"

#include "ast/statement/visitor/IStmtVisitor.hpp"
#include "error/Errors.hpp"

DoWhile::DoWhile(IStmtPtr body, IExprPtr cond)
: body_(std::move(body))
, cond_(std::move(cond)) 
{ }

IStmtPtr DoWhile::GetBody() const noexcept {
    return body_;
}

IExprPtr DoWhile::GetCond() const noexcept {
    return cond_;
}

void DoWhile::Accept(IStmtVisitor& visitor) const {
    visitor.Visit(*this);
}