#pragma once

#include "ast/expression/IExpr.hpp"
#include "ast/statement/IStmt.hpp"

class Write final : public IStmt {
public:
    explicit Write(IExprPtr expr);

    IExprPtr GetExpr() const noexcept;

    void Accept(IStmtVisitor& visitor) const override;

private:
    IExprPtr expr_;
};