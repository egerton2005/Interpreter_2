#pragma once

#include "value/Value.hpp"
#include "IExpr.hpp"

class Const: public IExpr {
public:
    explicit Const(const Value& value);

    const Value& GetValue() const noexcept;

    void Accept(IExprVisitor& visitor) const override;
private:
    Value value_;
};