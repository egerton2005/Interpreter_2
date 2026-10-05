#include "Skip.hpp"

#include "ast/statement/visitor/IStmtVisitor.hpp"

void Skip::Accept(IStmtVisitor& visitor) const {
    visitor.Visit(*this);
}