#pragma once

#include <cstddef>
#include <string>

class Position {
public:
    Position(std::size_t line, std::size_t column) noexcept;

    std::size_t GetLine() const noexcept;
    std::size_t GetColumn() const noexcept;

    std::string ToString() const;

    bool operator==(const Position& other) const noexcept = default;

private:
    std::size_t line_;
    std::size_t column_;
};