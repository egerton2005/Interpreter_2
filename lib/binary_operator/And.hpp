#pragma once

#include "IBinaryOperator.hpp"

class And final : public IBinaryOperator {
public:
    Value Apply(const Value& lhs, const Value& rhs) const override {
        return Value(static_cast<bool>(lhs) && static_cast<bool>(rhs));
    }
};