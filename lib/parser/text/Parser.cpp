#include "Parser.hpp"

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include "ast/expression/BinOp.hpp"
#include "ast/expression/Const.hpp"
#include "ast/expression/Var.hpp"
#include "ast/statement/Block.hpp"
#include "error/Errors.hpp"

const std::unordered_map<Token, int> Parser::kPrecedence = {
	{Token::Or, 1},
	{Token::And, 2},
	{Token::Less, 3},
	{Token::LessEqual, 3},
	{Token::Greater, 3},
	{Token::GreaterEqual, 3},
	{Token::Equal, 3},
	{Token::NotEqual, 3},
	{Token::Plus, 4},
	{Token::Minus, 4},
	{Token::Star, 5},
	{Token::Slash, 5},
	{Token::Percent, 5},
};

Parser::Parser(Lexer lexer, StatementParserRegistry statements, BinaryOperatorRegistryPtr operators)
: lexer_(std::move(lexer))
, statements_(std::move(statements))
, operators_(std::move(operators))
{ }

IStmtPtr Parser::ParseProgram() {
	IStmtPtr program = ParseSequence(Token::EndOfFile);
	lexer_.Expect(Token::EndOfFile);
	return program;
}

IStmtPtr Parser::ParseSequence(Token terminator) {
	std::vector<IStmtPtr> statements;
	statements.push_back(ParseStatement());

	while (true) {
		lexer_.Match(Token::Semicolon);
		if (lexer_.Peek().Is(terminator)) {
			break;
		}
		statements.push_back(ParseStatement());
	}

	if (statements.size() == 1) {
		return statements.front();
	}
	return std::make_shared<Block>(std::move(statements));
}

IStmtPtr Parser::ParseStatement() {
	const Lexeme& current = lexer_.Peek();
	const IStatementParser* parser = statements_.Find(current.GetKind());
	if (!parser) {
		throw ParseError(current.GetPosition(), "expected statement, got '" + current.GetText() + "'");
	}
	return parser->Parse(*this);
}

IExprPtr Parser::ParseExpression(int min_precedence) {
	IExprPtr lhs = ParsePrimary();

	while (true) {
		const std::optional<int> precedence = BinaryPrecedence(lexer_.Peek().GetKind());
		if (!precedence || *precedence < min_precedence) {
			return lhs;
		}

		const Lexeme op = lexer_.Advance();
		IExprPtr rhs = ParseExpression(*precedence + 1);
		lhs = std::make_shared<BinOp>(operators_->Get(op.GetText()), std::move(lhs), std::move(rhs));
	}
}

Lexer& Parser::GetLexer() noexcept {
	return lexer_;
}

const BinaryOperatorRegistry& Parser::GetOperators() const noexcept {
	return *operators_;
}

IExprPtr Parser::ParsePrimary() {
	switch (lexer_.Peek().GetKind()) {
		case Token::Number: return ParseNumber();
		case Token::Ident: return std::make_shared<Var>(lexer_.Advance().GetText());
		case Token::LParen: return ParseParenthesized();
		default:
			throw ParseError(lexer_.Peek().GetPosition(), "expected expression, got '" + lexer_.Peek().GetText() + "'");
	}
}

IExprPtr Parser::ParseParenthesized() {
	lexer_.Expect(Token::LParen);
	IExprPtr inner = ParseExpression();
	lexer_.Expect(Token::RParen);
	return inner;
}

IExprPtr Parser::ParseNumber() {
	const Lexeme number = lexer_.Expect(Token::Number);
	try {
		const long long value = std::stoll(number.GetText());
		if (value > INT32_MAX) {
			throw std::out_of_range("");
		}
		return std::make_shared<Const>(Value(static_cast<std::int32_t>(value)));
	} catch (const std::out_of_range&) {
		throw ParseError(number.GetPosition(), "number is out of range: " + number.GetText());
	}
}

std::optional<int> Parser::BinaryPrecedence(Token token) const {
	const auto entry = kPrecedence.find(token);
	if (entry == kPrecedence.end()) {
		return std::nullopt;
	}
	return entry->second;
}