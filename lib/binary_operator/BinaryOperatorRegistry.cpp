#include "binary_operator/BinaryOperatorRegistry.hpp"

#include <utility>

#include "error/Errors.hpp"

BinaryOperator::BinaryOperator(Function function)
: function_(std::move(function))
{ }

Value BinaryOperator::Apply(const Value& lhs, const Value& rhs) const {
    return function_(lhs, rhs);
}

BinaryOperatorRegistry::Builder& BinaryOperatorRegistry::Builder::Add(
    std::string symbol,
    BinaryOperator::Function function
) {
    auto op = std::make_shared<BinaryOperator>(std::move(function));
    const bool inserted = operators_.emplace(std::move(symbol), std::move(op)).second;
    if (!inserted) {
        throw RuntimeError("duplicate binary operator");
    }
    return *this;
}

BinaryOperatorRegistry BinaryOperatorRegistry::Builder::Build() {
    return BinaryOperatorRegistry(std::move(operators_));
}

BinaryOperatorPtr BinaryOperatorRegistry::Get(std::string_view symbol) const {
    const auto iterator = operators_.find(std::string(symbol));
    if (iterator == operators_.end()) {
        throw SyntaxError("unknown binary operator: " + std::string(symbol));
    }
    return iterator->second;
}

BinaryOperatorRegistry::BinaryOperatorRegistry(
    std::unordered_map<std::string, BinaryOperatorPtr> operators
)
: operators_(std::move(operators))
{ }

BinaryOperatorRegistryPtr CreateDefaultBinaryOperators() {
    BinaryOperatorRegistry::Builder builder;
    builder.Add("+", [](const Value& lhs, const Value& rhs) { return lhs + rhs; })
           .Add("-", [](const Value& lhs, const Value& rhs) { return lhs - rhs; })
           .Add("*", [](const Value& lhs, const Value& rhs) { return lhs * rhs; })
           .Add("/", [](const Value& lhs, const Value& rhs) { return lhs / rhs; })
           .Add("%", [](const Value& lhs, const Value& rhs) { return lhs % rhs; })
           .Add("<", [](const Value& lhs, const Value& rhs) { return Value(lhs < rhs); })
           .Add("<=", [](const Value& lhs, const Value& rhs) { return Value(lhs <= rhs); })
           .Add(">", [](const Value& lhs, const Value& rhs) { return Value(lhs > rhs); })
           .Add(">=", [](const Value& lhs, const Value& rhs) { return Value(lhs >= rhs); })
           .Add("==", [](const Value& lhs, const Value& rhs) { return Value(lhs == rhs); })
           .Add("!=", [](const Value& lhs, const Value& rhs) { return Value(lhs != rhs); })
           .Add("&&", [](const Value& lhs, const Value& rhs) {
               return Value(static_cast<bool>(lhs) && static_cast<bool>(rhs));
           })
           .Add("!!", [](const Value& lhs, const Value& rhs) {
               return Value(static_cast<bool>(lhs) || static_cast<bool>(rhs));
           });

    return std::make_shared<BinaryOperatorRegistry>(builder.Build());
}
