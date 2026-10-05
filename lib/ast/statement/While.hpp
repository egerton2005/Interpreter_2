#pragma once

#include "ast/expression/IExpr.hpp"
#include "ast/statement/IStmt.hpp"

class While final : public IStmt {
public:
    While(IExprPtr cond, IStmtPtr body);

    IExprPtr GetCond() const noexcept;
    IStmtPtr GetBody() const noexcept;

    void Accept(IStmtVisitor& visitor) const override;

private:
    IExprPtr cond_;
    IStmtPtr body_;
};