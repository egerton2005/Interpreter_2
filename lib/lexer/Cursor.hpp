#pragma once

#include <cstddef>
#include <string_view>

#include "Position.hpp"

class Cursor {
public:
    explicit Cursor(std::string_view source) noexcept;

    bool AtEnd() const noexcept;
    char Peek(std::size_t offset = 0) const noexcept;
    bool StartsWith(std::string_view prefix) const noexcept;

    char Advance();
    void Skip(std::size_t count);

    Position Where() const noexcept;

private:
    std::string_view source_;
    std::size_t offset_ = 0;
    std::size_t line_ = 1;
    std::size_t column_ = 1;
};