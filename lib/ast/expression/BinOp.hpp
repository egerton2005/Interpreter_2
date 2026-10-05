#pragma once

#include "IExpr.hpp"
#include "binary_operator/IBinaryOperator.hpp"

class BinOp: public IExpr {
public:
    BinOp(IBinaryOperatorPtr op, IExprPtr lhs, IExprPtr rhs);

    IBinaryOperatorPtr GetOperator() const noexcept;
    IExprPtr GetLhs() const noexcept;
    IExprPtr GetRhs() const noexcept;

    void Accept(IExprVisitor& visitor) const override;
private:
    IBinaryOperatorPtr op_;
    IExprPtr lhs_;
    IExprPtr rhs_;
};