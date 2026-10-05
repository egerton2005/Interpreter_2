#pragma once

#include "ast/expression/IExpr.hpp"
#include "ast/statement/IStmt.hpp"

class DoWhile final : public IStmt {
public:
    DoWhile(IStmtPtr body, IExprPtr cond);

    IStmtPtr GetBody() const noexcept;
    IExprPtr GetCond() const noexcept;

    void Accept(IStmtVisitor& visitor) const override;

private:
    IStmtPtr body_;
    IExprPtr cond_;
};