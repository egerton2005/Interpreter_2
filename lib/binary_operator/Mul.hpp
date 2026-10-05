#pragma once

#include "IBinaryOperator.hpp"

class Mul final : public IBinaryOperator {
public:
    Value Apply(const Value& lhs, const Value& rhs) const override {
        return lhs * rhs;
    }
};