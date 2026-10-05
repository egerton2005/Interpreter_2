#include "Position.hpp"

Position::Position(std::size_t line, std::size_t column) noexcept
: line_(line)
, column_(column)
{ }

std::size_t Position::GetLine() const noexcept {
    return line_;
}

std::size_t Position::GetColumn() const noexcept {
    return column_;
}

std::string Position::ToString() const {
    return std::to_string(line_) + ":" + std::to_string(column_);
}