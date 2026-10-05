#pragma once

#include "ast/statement/IStmt.hpp"

class Skip final : public IStmt {
public:
    void Accept(IStmtVisitor& visitor) const override;
};