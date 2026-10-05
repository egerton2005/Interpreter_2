#pragma once

#include <memory>

#include "ast/statement/IStmt.hpp"

class Parser;

class IStatementParser {
public:
	virtual IStmtPtr Parse(Parser& parser) const = 0;
	virtual ~IStatementParser() = default;
};

using IStatementParserPtr = std::shared_ptr<const IStatementParser>;