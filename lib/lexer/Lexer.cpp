#include "Lexer.hpp"

#include <cctype>
#include <utility>

#include "error/Errors.hpp"

const std::unordered_map<std::string_view, Token> Lexer::kKeywords = {
	{"skip", Token::Skip},
	{"read", Token::Read},
	{"write", Token::Write},
	{"if", Token::If},
	{"elif", Token::Elif},
	{"else", Token::Else},
	{"while", Token::While},
	{"do", Token::Do},
	{"for", Token::For},
};

Lexer::Lexer(std::string_view source)
: cursor_(source)
, current_(Read())
{ }

const Lexeme& Lexer::Peek() const noexcept {
	return current_;
}

Lexeme Lexer::Advance() {
	Lexeme consumed = std::move(current_);
	current_ = Read();
	return consumed;
}

bool Lexer::Match(Token token) {
	if (!current_.Is(token)) {
		return false;
	}
	Advance();
	return true;
}

Lexeme Lexer::Expect(Token token) {
	if (!current_.Is(token)) {
		throw ParseError(current_.GetPosition(), "expected " + Name(token) + ", got '" + current_.GetText() + "'");
	}
	return Advance();
}

std::string Lexer::Name(Token token) {
	switch (token) {
		case Token::Ident: return "identifier";
		case Token::Number: return "number";
		case Token::Skip: return "'skip'";
		case Token::Read: return "'read'";
		case Token::Write: return "'write'";
		case Token::If: return "'if'";
		case Token::Elif: return "'elif'";
		case Token::Else: return "'else'";
		case Token::While: return "'while'";
		case Token::Do: return "'do'";
		case Token::For: return "'for'";
		case Token::LParen: return "'('";
		case Token::RParen: return "')'";
		case Token::LBrace: return "'{'";
		case Token::RBrace: return "'}'";
		case Token::Semicolon: return "';'";
		case Token::Assign: return "'='";
		case Token::Plus: return "'+'";
		case Token::Minus: return "'-'";
		case Token::Star: return "'*'";
		case Token::Slash: return "'/'";
		case Token::Percent: return "'%'";
		case Token::Less: return "'<'";
		case Token::LessEqual: return "'<='";
		case Token::Greater: return "'>'";
		case Token::GreaterEqual: return "'>='";
		case Token::Equal: return "'=='";
		case Token::NotEqual: return "'!='";
		case Token::And: return "'&&'";
		case Token::Or: return "'!!'";
		case Token::PlusAssign: return "'+='";
		case Token::MinusAssign: return "'-='";
		case Token::StarAssign: return "'*='";
		case Token::SlashAssign: return "'/='";
		case Token::PercentAssign: return "'%='";
		case Token::EndOfFile: return "end of input";
	}
	return "unknown token";
}

Lexeme Lexer::Read() {
	SkipTrivia();

	if (cursor_.AtEnd()) {
		return Lexeme(Token::EndOfFile, "", cursor_.Where());
	}
	if (IsDigit(cursor_.Peek())) {
		return ReadNumber();
	}
	if (IsIdentStart(cursor_.Peek())) {
		return ReadWord();
	}
	return ReadSymbol();
}

bool Lexer::IsDigit(char c) noexcept {
	return std::isdigit(static_cast<unsigned char>(c));
}

bool Lexer::IsIdentStart(char c) noexcept {
	return c >= 'a' && c <= 'z';
}

bool Lexer::IsIdentPart(char c) noexcept {
	return std::isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '\'';
}

void Lexer::SkipTrivia() {
	while (!cursor_.AtEnd()) {
		if (std::isspace(static_cast<unsigned char>(cursor_.Peek()))) {
			cursor_.Advance();
		} else if (cursor_.StartsWith("--")) {
			SkipLineComment();
		} else if (cursor_.StartsWith("(*")) {
			SkipBlockComment();
		} else {
			return;
		}
	}
}

void Lexer::SkipLineComment() {
	while (!cursor_.AtEnd() && cursor_.Peek() != '\n') {
		cursor_.Advance();
	}
}

void Lexer::SkipBlockComment() {
	const Position start = cursor_.Where();
	cursor_.Skip(2);
	while (!cursor_.StartsWith("*)")) {
		if (cursor_.AtEnd()) {
			throw ParseError(start, "unterminated comment");
		}
		cursor_.Advance();
	}
	cursor_.Skip(2);
}

Lexeme Lexer::ReadNumber() {
	const Position start = cursor_.Where();
	std::string text;
	while (IsDigit(cursor_.Peek())) {
		text += cursor_.Advance();
	}
	return Lexeme(Token::Number, std::move(text), start);
}

Lexeme Lexer::ReadWord() {
	const Position start = cursor_.Where();
	std::string text;
	while (IsIdentPart(cursor_.Peek())) {
		text += cursor_.Advance();
	}
	const auto keyword = kKeywords.find(text);
	const Token kind = keyword == kKeywords.end() ? Token::Ident : keyword->second;
	return Lexeme(kind, std::move(text), start);
}

Lexeme Lexer::Single(Token token, char symbol, Position start) {
	return Lexeme(token, std::string(1, symbol), start);
}

Lexeme Lexer::WithEqual(Token token, char symbol, Position start) {
	cursor_.Advance();
	return Lexeme(token, std::string{symbol, '='}, start);
}

Lexeme Lexer::ReadSymbol() {
	const Position start = cursor_.Where();
	const char first = cursor_.Advance();
	const bool followed_by_equal = cursor_.Peek() == '=';

	switch (first) {
		case '(': return Single(Token::LParen, first, start);
		case ')': return Single(Token::RParen, first, start);
		case '{': return Single(Token::LBrace, first, start);
		case '}': return Single(Token::RBrace, first, start);
		case ';': return Single(Token::Semicolon, first, start);
		case '+': return followed_by_equal ? WithEqual(Token::PlusAssign, first, start) : Single(Token::Plus, first, start);
		case '-': return followed_by_equal ? WithEqual(Token::MinusAssign, first, start) : Single(Token::Minus, first, start);
		case '*': return followed_by_equal ? WithEqual(Token::StarAssign, first, start) : Single(Token::Star, first, start);
		case '/': return followed_by_equal ? WithEqual(Token::SlashAssign, first, start) : Single(Token::Slash, first, start);
		case '%': return followed_by_equal ? WithEqual(Token::PercentAssign, first, start) : Single(Token::Percent, first, start);
		case '<': return followed_by_equal ? WithEqual(Token::LessEqual, first, start) : Single(Token::Less, first, start);
		case '>': return followed_by_equal ? WithEqual(Token::GreaterEqual, first, start) : Single(Token::Greater, first, start);
		case '=': return followed_by_equal ? WithEqual(Token::Equal, first, start) : Single(Token::Assign, first, start);
		case '!':
			if (followed_by_equal) {
				return WithEqual(Token::NotEqual, first, start);
			}
			if (cursor_.Peek() == '!') {
				cursor_.Advance();
				return Lexeme(Token::Or, "!!", start);
			}
			break;
		case '&':
			if (cursor_.Peek() == '&') {
				cursor_.Advance();
				return Lexeme(Token::And, "&&", start);
			}
			break;
	}

	throw ParseError(start, std::string("unexpected character '") + first + "'");
}