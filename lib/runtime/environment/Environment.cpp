#include "runtime/environment/Environment.hpp"

#include <utility>

#include "error/Errors.hpp"

Value Environment::Get(std::string_view name) const {
    const auto iterator = variables_.find(std::string(name));
    if (iterator == variables_.end()) {
        throw UndefinedVariable(std::string(name));
    }
    return iterator->second;
}

void Environment::Set(std::string name, Value value) {
    variables_.insert_or_assign(std::move(name), value);
}

void Environment::Clear() noexcept {
    variables_.clear();
}
