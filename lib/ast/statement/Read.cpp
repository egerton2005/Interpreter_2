#include "Read.hpp"

#include "ast/statement/visitor/IStmtVisitor.hpp"

Read::Read(std::string name) 
: name_(std::move(name)) 
{ }

const std::string& Read::GetName() const noexcept {
    return name_;
}

void Read::Accept(IStmtVisitor& visitor) const {
    visitor.Visit(*this);
}