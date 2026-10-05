#include "StatementParserRegistry.hpp"

#include <utility>

#include "error/Errors.hpp"

StatementParserRegistry::Builder& StatementParserRegistry::Builder::Add(Token token, IStatementParserPtr parser) {
	if (!parser) {
		throw LogicError("StatementParserRegistry::Builder::Add: null parser");
	}
	if (!parsers_.emplace(token, std::move(parser)).second) {
		throw LogicError("StatementParserRegistry::Builder::Add: duplicate token");
	}
	return *this;
}

StatementParserRegistry StatementParserRegistry::Builder::Build() {
	return StatementParserRegistry(std::move(parsers_));
}

const IStatementParser* StatementParserRegistry::Find(Token token) const noexcept {
	const auto entry = parsers_.find(token);
	return entry == parsers_.end() ? nullptr : entry->second.get();
}

StatementParserRegistry::StatementParserRegistry(std::unordered_map<Token, IStatementParserPtr> parsers)
: parsers_(std::move(parsers))
{ }