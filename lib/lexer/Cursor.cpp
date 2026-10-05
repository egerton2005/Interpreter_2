#include "Cursor.hpp"

#include "error/Errors.hpp"

Cursor::Cursor(std::string_view source) noexcept
: source_(source)
{ }

bool Cursor::AtEnd() const noexcept {
    return offset_ >= source_.size();
}

char Cursor::Peek(std::size_t offset) const noexcept {
    const std::size_t index = offset_ + offset;
    return index < source_.size() ? source_[index] : '\0';
}

bool Cursor::StartsWith(std::string_view prefix) const noexcept {
    return source_.substr(offset_).starts_with(prefix);
}

char Cursor::Advance() {
    if (AtEnd()) {
        throw LogicError("Cursor::Advance: past end of input");
    }

    const char current = source_[offset_++];
    if (current == '\n') {
        ++line_;
        column_ = 1;
    } else {
        ++column_;
    }
    return current;
}

void Cursor::Skip(std::size_t count) {
    for (std::size_t i = 0; i < count; ++i) {
        Advance();
    }
}

Position Cursor::Where() const noexcept {
    return Position(line_, column_);
}