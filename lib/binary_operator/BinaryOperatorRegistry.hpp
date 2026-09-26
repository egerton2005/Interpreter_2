#pragma once

#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

#include "value/Value.hpp"

class BinaryOperator {
public:
    using Function = std::function<Value(const Value&, const Value&)>;

    explicit BinaryOperator(Function function);
    Value Apply(const Value& lhs, const Value& rhs) const;

private:
    Function function_;
};

using BinaryOperatorPtr = std::shared_ptr<const BinaryOperator>;

class BinaryOperatorRegistry {
public:
    class Builder {
    public:
        Builder& Add(std::string symbol, BinaryOperator::Function function);
        BinaryOperatorRegistry Build();

    private:
        std::unordered_map<std::string, BinaryOperatorPtr> operators_;
    };

    BinaryOperatorPtr Get(std::string_view symbol) const;

private:
    explicit BinaryOperatorRegistry(
        std::unordered_map<std::string, BinaryOperatorPtr> operators
    );

    std::unordered_map<std::string, BinaryOperatorPtr> operators_;
};

using BinaryOperatorRegistryPtr = std::shared_ptr<const BinaryOperatorRegistry>;

BinaryOperatorRegistryPtr CreateDefaultBinaryOperators();
