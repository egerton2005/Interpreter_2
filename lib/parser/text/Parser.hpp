#pragma once

#include <optional>
#include <unordered_map>

#include "ast/expression/IExpr.hpp"
#include "ast/statement/IStmt.hpp"
#include "binary_operator/BinaryOperatorRegistry.hpp"
#include "lexer/Lexer.hpp"
#include "statement_parser/StatementParserRegistry.hpp"

class Parser {
public:
	Parser(Lexer lexer, StatementParserRegistry statements, BinaryOperatorRegistryPtr operators);

	IStmtPtr ParseProgram();
	IStmtPtr ParseSequence(Token terminator);
	IStmtPtr ParseStatement();
	IExprPtr ParseExpression(int min_precedence = 1);

	Lexer& GetLexer() noexcept;
	const BinaryOperatorRegistry& GetOperators() const noexcept;

private:
	IExprPtr ParsePrimary();
	IExprPtr ParseParenthesized();
	IExprPtr ParseNumber();
	std::optional<int> BinaryPrecedence(Token token) const;

	Lexer lexer_;
	StatementParserRegistry statements_;
	BinaryOperatorRegistryPtr operators_;

	static const std::unordered_map<Token, int> kPrecedence;
};