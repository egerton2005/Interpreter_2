#include "Block.hpp"

#include "ast/statement/visitor/IStmtVisitor.hpp"
#include "error/Errors.hpp"

Block::Block(std::vector<IStmtPtr> statements)
: statements_(std::move(statements))
{ }

const std::vector<IStmtPtr>& Block::GetStatements() const noexcept{
    return statements_;
}

void Block::Accept(IStmtVisitor& visitor) const {
    visitor.Visit(*this);
}