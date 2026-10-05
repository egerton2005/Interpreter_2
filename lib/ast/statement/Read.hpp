#pragma once

#include <string>

#include "ast/statement/IStmt.hpp"

class Read final : public IStmt {
public:
    explicit Read(std::string name);

    const std::string& GetName() const noexcept;

    void Accept(IStmtVisitor& visitor) const override;

private:
    std::string name_;
};