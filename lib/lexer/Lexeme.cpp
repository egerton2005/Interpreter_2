#include "Lexeme.hpp"

Lexeme::Lexeme(Token kind, std::string text, Position position)
: kind_(kind)
, text_(std::move(text))
, position_(position)
{ }

Token Lexeme::GetKind() const noexcept {
    return kind_;
}

const std::string& Lexeme::GetText() const noexcept {
    return text_;
}

Position Lexeme::GetPosition() const noexcept {
    return position_;
}

bool Lexeme::Is(Token kind) const noexcept {
    return kind_ == kind;
}