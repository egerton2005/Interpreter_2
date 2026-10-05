#pragma once

#include <vector>

#include "ast/statement/IStmt.hpp"

class Block final : public IStmt {
public:
    explicit Block(std::vector<IStmtPtr> statements);

    const std::vector<IStmtPtr>& GetStatements() const noexcept;

    void Accept(IStmtVisitor& visitor) const override;

private:
    std::vector<IStmtPtr> statements_;
};