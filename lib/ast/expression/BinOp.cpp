#include "BinOp.hpp"

#include "visitor/IExprVisitor.hpp"
#include "error/Errors.hpp"

BinOp::BinOp(IBinaryOperatorPtr op, IExprPtr lhs, IExprPtr rhs)
: op_(std::move(op))
, lhs_(std::move(lhs))
, rhs_(std::move(rhs)) 
{ }

IBinaryOperatorPtr BinOp::GetOperator() const noexcept {
    return op_;
}

IExprPtr BinOp::GetLhs() const noexcept {
    return lhs_;
}

IExprPtr BinOp::GetRhs() const noexcept {
    return rhs_;
}

void BinOp::Accept(IExprVisitor& visitor) const {
    visitor.Visit(*this);
}