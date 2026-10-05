#pragma once

#include "ast/expression/IExpr.hpp"
#include "ast/statement/IStmt.hpp"

class If final : public IStmt {
public:
    If(IExprPtr cond, IStmtPtr thenBranch, IStmtPtr elseBranch);

    IExprPtr GetCond() const noexcept;
    IStmtPtr GetThenBranch() const noexcept;
    IStmtPtr GetElseBranch() const noexcept;

    void Accept(IStmtVisitor& visitor) const override;

private:
    IExprPtr cond_;
    IStmtPtr then_;
    IStmtPtr else_;
};