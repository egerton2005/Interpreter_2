#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

#include "value/Value.hpp"

class Environment {
public:
    Value Get(std::string_view name) const;
    void Set(std::string name, Value value);
    void Clear() noexcept;

private:
    std::unordered_map<std::string, Value> variables_;
};
