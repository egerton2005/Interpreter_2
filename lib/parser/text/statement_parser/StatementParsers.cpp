#include "StatementParsers.hpp"

#include <memory>
#include <utility>
#include <vector>

#include "ast/expression/BinOp.hpp"
#include "ast/expression/Var.hpp"
#include "ast/statement/Assign.hpp"
#include "ast/statement/Block.hpp"
#include "ast/statement/DoWhile.hpp"
#include "ast/statement/If.hpp"
#include "ast/statement/Read.hpp"
#include "ast/statement/Skip.hpp"
#include "ast/statement/While.hpp"
#include "ast/statement/Write.hpp"
#include "error/Errors.hpp"
#include "parser/text/Parser.hpp"

IStmtPtr SkipParser::Parse(Parser& parser) const {
	parser.GetLexer().Expect(Token::Skip);
	return std::make_shared<Skip>();
}

IStmtPtr BlockParser::Parse(Parser& parser) const {
	Lexer& lexer = parser.GetLexer();
	lexer.Expect(Token::LBrace);
	IStmtPtr body = parser.ParseSequence(Token::RBrace);
	lexer.Expect(Token::RBrace);
	return body;
}

IStmtPtr AssignParser::Parse(Parser& parser) const {
	Lexer& lexer = parser.GetLexer();
	const Lexeme name = lexer.Expect(Token::Ident);

	if (lexer.Match(Token::Assign)) {
		return std::make_shared<Assign>(name.GetText(), parser.ParseExpression());
	}

	const Lexeme op = lexer.Advance();
	const std::string symbol = CompoundOperatorSymbol(op);
	IExprPtr rhs = parser.ParseExpression();
	IExprPtr combined = std::make_shared<BinOp>(parser.GetOperators().Get(symbol), std::make_shared<Var>(name.GetText()), std::move(rhs));
	return std::make_shared<Assign>(name.GetText(), std::move(combined));
}

std::string AssignParser::CompoundOperatorSymbol(const Lexeme& lexeme) {
	switch (lexeme.GetKind()) {
		case Token::PlusAssign:
		case Token::MinusAssign:
		case Token::StarAssign:
		case Token::SlashAssign:
		case Token::PercentAssign:
			return lexeme.GetText().substr(0, lexeme.GetText().size() - 1);
		default:
			throw ParseError(lexeme.GetPosition(), "expected '=' or compound assignment, got '" + lexeme.GetText() + "'");
	}
}

IStmtPtr ReadStatementParser::Parse(Parser& parser) const {
	Lexer& lexer = parser.GetLexer();
	lexer.Expect(Token::Read);
	lexer.Expect(Token::LParen);
	const Lexeme name = lexer.Expect(Token::Ident);
	lexer.Expect(Token::RParen);
	return std::make_shared<Read>(name.GetText());
}

IStmtPtr WriteStatementParser::Parse(Parser& parser) const {
	Lexer& lexer = parser.GetLexer();
	lexer.Expect(Token::Write);
	lexer.Expect(Token::LParen);
	IExprPtr expr = parser.ParseExpression();
	lexer.Expect(Token::RParen);
	return std::make_shared<Write>(std::move(expr));
}

IStmtPtr IfParser::Parse(Parser& parser) const {
	parser.GetLexer().Expect(Token::If);
	return ParseBranch(parser);
}

IStmtPtr IfParser::ParseBranch(Parser& parser) {
	Lexer& lexer = parser.GetLexer();
	lexer.Expect(Token::LParen);
	IExprPtr cond = parser.ParseExpression();
	lexer.Expect(Token::RParen);
	IStmtPtr then_branch = parser.ParseStatement();
	IStmtPtr else_branch = ParseElseChain(parser);
	return std::make_shared<If>(std::move(cond), std::move(then_branch), std::move(else_branch));
}

IStmtPtr IfParser::ParseElseChain(Parser& parser) {
	Lexer& lexer = parser.GetLexer();
	if (lexer.Match(Token::Elif)) {
		return ParseBranch(parser);
	}
	if (lexer.Match(Token::Else)) {
		return parser.ParseStatement();
	}
	return std::make_shared<Skip>();
}

IStmtPtr WhileParser::Parse(Parser& parser) const {
	Lexer& lexer = parser.GetLexer();
	lexer.Expect(Token::While);
	lexer.Expect(Token::LParen);
	IExprPtr cond = parser.ParseExpression();
	lexer.Expect(Token::RParen);
	IStmtPtr body = parser.ParseStatement();
	return std::make_shared<While>(std::move(cond), std::move(body));
}

IStmtPtr DoWhileParser::Parse(Parser& parser) const {
	Lexer& lexer = parser.GetLexer();
	lexer.Expect(Token::Do);
	IStmtPtr body = parser.ParseStatement();
	lexer.Expect(Token::While);
	lexer.Expect(Token::LParen);
	IExprPtr cond = parser.ParseExpression();
	lexer.Expect(Token::RParen);
	return std::make_shared<DoWhile>(std::move(body), std::move(cond));
}

IStmtPtr ForParser::Parse(Parser& parser) const {
	Lexer& lexer = parser.GetLexer();
	lexer.Expect(Token::For);
	lexer.Expect(Token::LParen);
	IStmtPtr init = parser.ParseStatement();
	lexer.Expect(Token::Semicolon);
	IExprPtr cond = parser.ParseExpression();
	lexer.Expect(Token::Semicolon);
	IStmtPtr step = parser.ParseStatement();
	lexer.Expect(Token::RParen);
	IStmtPtr body = parser.ParseStatement();

	std::vector<IStmtPtr> loop_body;
	loop_body.push_back(std::move(body));
	loop_body.push_back(std::move(step));
	IStmtPtr loop = std::make_shared<While>(std::move(cond), std::make_shared<Block>(std::move(loop_body)));

	std::vector<IStmtPtr> result;
	result.push_back(std::move(init));
	result.push_back(std::move(loop));
	return std::make_shared<Block>(std::move(result));
}