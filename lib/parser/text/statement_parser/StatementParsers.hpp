#pragma once

#include <string>

#include "IStatementParser.hpp"
#include "lexer/Lexeme.hpp"

class SkipParser final : public IStatementParser {
public:
	IStmtPtr Parse(Parser& parser) const override;
};

class BlockParser final : public IStatementParser {
public:
	IStmtPtr Parse(Parser& parser) const override;
};

class AssignParser final : public IStatementParser {
public:
	IStmtPtr Parse(Parser& parser) const override;

private:
	static std::string CompoundOperatorSymbol(const Lexeme& lexeme);
};

class ReadStatementParser final : public IStatementParser {
public:
	IStmtPtr Parse(Parser& parser) const override;
};

class WriteStatementParser final : public IStatementParser {
public:
	IStmtPtr Parse(Parser& parser) const override;
};

class IfParser final : public IStatementParser {
public:
	IStmtPtr Parse(Parser& parser) const override;

private:
	static IStmtPtr ParseBranch(Parser& parser);
	static IStmtPtr ParseElseChain(Parser& parser);
};

class WhileParser final : public IStatementParser {
public:
	IStmtPtr Parse(Parser& parser) const override;
};

class DoWhileParser final : public IStatementParser {
public:
	IStmtPtr Parse(Parser& parser) const override;
};

class ForParser final : public IStatementParser {
public:
	IStmtPtr Parse(Parser& parser) const override;
};