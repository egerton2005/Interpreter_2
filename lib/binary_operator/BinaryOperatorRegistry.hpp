#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

#include "binary_operator/IBinaryOperator.hpp"

class BinaryOperatorRegistry {
public:
    class Builder {
    public:
        Builder& Add(std::string_view symbol, IBinaryOperatorPtr op);

        BinaryOperatorRegistry Build();

    private:
        std::unordered_map<std::string, IBinaryOperatorPtr> operators_;
    };

    IBinaryOperatorPtr Get(std::string_view symbol) const;

private:
    explicit BinaryOperatorRegistry(std::unordered_map<std::string, IBinaryOperatorPtr> operators);

    std::unordered_map<std::string, IBinaryOperatorPtr> operators_;
};

using BinaryOperatorRegistryPtr = std::shared_ptr<BinaryOperatorRegistry>;