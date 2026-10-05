#pragma once

#include <string>

#include "IExpr.hpp"

class Var final: public IExpr {
public:
    explicit Var(std::string name);

    const std::string& GetName() const noexcept;

    void Accept(IExprVisitor& visitor) const override;

private:
    std::string name_;
};