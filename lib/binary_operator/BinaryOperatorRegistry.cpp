#include "BinaryOperatorRegistry.hpp"

#include <utility>

#include "error/Errors.hpp"

BinaryOperatorRegistry::Builder& BinaryOperatorRegistry::Builder::Add(std::string_view symbol, IBinaryOperatorPtr op) {
    bool inserted = operators_.emplace(std::string(symbol), std::move(op)).second;
    
    if (!inserted) {
        throw LogicError("BinaryOperatorRegistry::Builder::Add: duplicate operator symbol " + std::string(symbol));
    }

    return *this;
}

BinaryOperatorRegistry BinaryOperatorRegistry::Builder::Build() {
    return BinaryOperatorRegistry(std::move(operators_));
}

IBinaryOperatorPtr BinaryOperatorRegistry::Get(std::string_view symbol) const {
    auto entry = operators_.find(std::string(symbol));

    if (entry == operators_.end()) {
        throw SyntaxError("unknown binary operator: " + std::string(symbol));
    }

    return entry->second;
}

BinaryOperatorRegistry::BinaryOperatorRegistry(
std::unordered_map<std::string, IBinaryOperatorPtr> operators)
: operators_(std::move(operators))
{ }