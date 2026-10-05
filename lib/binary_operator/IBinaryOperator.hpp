#pragma once

#include <memory>

#include "value/Value.hpp"

class IBinaryOperator {
public:
    IBinaryOperator(const IBinaryOperator&) = delete;
    IBinaryOperator& operator=(const IBinaryOperator&) = delete;
    
    virtual Value Apply(const Value& lhs, const Value& rhs) const = 0;
    
    virtual ~IBinaryOperator() = default;

protected:
    IBinaryOperator() = default;
};

using IBinaryOperatorPtr = std::shared_ptr<IBinaryOperator>;