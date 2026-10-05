#pragma once

#include "IBinaryOperator.hpp"

class Lt final : public IBinaryOperator {
public:
    Value Apply(const Value& lhs, const Value& rhs) const override {
        return Value(lhs < rhs);
    }
};