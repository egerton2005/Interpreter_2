#pragma once

#include <string>

#include "Position.hpp"
#include "Token.hpp"

class Lexeme {
public:
    Lexeme(Token kind, std::string text, Position position);

    Token GetKind() const noexcept;
    const std::string& GetText() const noexcept;
    Position GetPosition() const noexcept;

    bool Is(Token kind) const noexcept;

private:
    Token kind_;
    std::string text_;
    Position position_;
};