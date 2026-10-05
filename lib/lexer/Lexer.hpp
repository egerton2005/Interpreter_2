#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

#include "Cursor.hpp"
#include "Lexeme.hpp"

class Lexer {
public:
	explicit Lexer(std::string_view source);

	const Lexeme& Peek() const noexcept;
	Lexeme Advance();
	bool Match(Token token);
	Lexeme Expect(Token token);

	static std::string Name(Token token);

private:
	Lexeme Read();

	void SkipTrivia();
	void SkipLineComment();
	void SkipBlockComment();

	Lexeme ReadNumber();
	Lexeme ReadWord();
	Lexeme ReadSymbol();

	Lexeme Single(Token token, char symbol, Position start);
	Lexeme WithEqual(Token token, char symbol, Position start);

	static bool IsDigit(char c) noexcept;
	static bool IsIdentStart(char c) noexcept;
	static bool IsIdentPart(char c) noexcept;

	Cursor cursor_;
	Lexeme current_;

	static const std::unordered_map<std::string_view, Token> kKeywords;
};