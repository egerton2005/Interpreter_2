#pragma once

#include "IBinaryOperator.hpp"

class Le final : public IBinaryOperator {
public:
    Value Apply(const Value& lhs, const Value& rhs) const override {
        return Value(lhs <= rhs);
    }
};