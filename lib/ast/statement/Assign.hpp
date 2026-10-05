#pragma once

#include <string>

#include "ast/expression/IExpr.hpp"
#include "ast/statement/IStmt.hpp"

class Assign final : public IStmt {
public:
    Assign(std::string name, IExprPtr value);

    const std::string& GetName() const noexcept;
    IExprPtr GetValue() const noexcept;

    void Accept(IStmtVisitor& visitor) const override;

private:
    std::string name_;
    IExprPtr value_;
};