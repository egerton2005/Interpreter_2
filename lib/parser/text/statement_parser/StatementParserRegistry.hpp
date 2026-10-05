#pragma once

#include <unordered_map>

#include "IStatementParser.hpp"
#include "lexer/Token.hpp"

class StatementParserRegistry {
public:
	class Builder {
	public:
		Builder& Add(Token token, IStatementParserPtr parser);
		StatementParserRegistry Build();

	private:
		std::unordered_map<Token, IStatementParserPtr> parsers_;
	};

	const IStatementParser* Find(Token token) const noexcept;

private:
	explicit StatementParserRegistry(std::unordered_map<Token, IStatementParserPtr> parsers);

	std::unordered_map<Token, IStatementParserPtr> parsers_;
};