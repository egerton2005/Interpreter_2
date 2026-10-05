#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>

#include "binary_operator/Add.hpp"
#include "binary_operator/And.hpp"
#include "binary_operator/BinaryOperatorRegistry.hpp"
#include "binary_operator/Div.hpp"
#include "binary_operator/Eq.hpp"
#include "binary_operator/Ge.hpp"
#include "binary_operator/Gt.hpp"
#include "binary_operator/Le.hpp"
#include "binary_operator/Lt.hpp"
#include "binary_operator/Mod.hpp"
#include "binary_operator/Mul.hpp"
#include "binary_operator/Ne.hpp"
#include "binary_operator/Or.hpp"
#include "binary_operator/Sub.hpp"
#include "compiler/Compiler.hpp"
#include "error/Errors.hpp"
#include "lexer/Lexer.hpp"
#include "parser/text/Parser.hpp"
#include "parser/text/statement_parser/StatementParsers.hpp"
#include "runtime/Machine.hpp"
#include "runtime/io/StreamInput.hpp"
#include "runtime/io/StreamOutput.hpp"

std::string ReadFile(const std::string& path) {
	std::ifstream file(path);
	if (!file) {
		throw std::runtime_error("cannot open " + path);
	}
	return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

BinaryOperatorRegistryPtr CreateOperators() {
	BinaryOperatorRegistry::Builder builder;
	builder.Add("+", std::make_shared<Add>())
		.Add("-", std::make_shared<Sub>())
		.Add("*", std::make_shared<Mul>())
		.Add("/", std::make_shared<Div>())
		.Add("%", std::make_shared<Mod>())
		.Add("<", std::make_shared<Lt>())
		.Add("<=", std::make_shared<Le>())
		.Add(">", std::make_shared<Gt>())
		.Add(">=", std::make_shared<Ge>())
		.Add("==", std::make_shared<Eq>())
		.Add("!=", std::make_shared<Ne>())
		.Add("&&", std::make_shared<And>())
		.Add("!!", std::make_shared<Or>());
	return std::make_shared<BinaryOperatorRegistry>(builder.Build());
}

StatementParserRegistry CreateStatementParsers() {
	StatementParserRegistry::Builder builder;
	builder.Add(Token::Skip, std::make_shared<SkipParser>())
		.Add(Token::LBrace, std::make_shared<BlockParser>())
		.Add(Token::Ident, std::make_shared<AssignParser>())
		.Add(Token::Read, std::make_shared<ReadStatementParser>())
		.Add(Token::Write, std::make_shared<WriteStatementParser>())
		.Add(Token::If, std::make_shared<IfParser>())
		.Add(Token::While, std::make_shared<WhileParser>())
		.Add(Token::Do, std::make_shared<DoWhileParser>())
		.Add(Token::For, std::make_shared<ForParser>());
	return builder.Build();
}

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cerr << "usage: " << argv[0] << " <source>\n";
		return 2;
	}

	try {
		const std::string source = ReadFile(argv[1]);

		Parser parser(Lexer(source), CreateStatementParsers(), CreateOperators());
		IStmtPtr ast = parser.ParseProgram();

		Compiler compiler;
		Program program = compiler.Compile(*ast);

		Machine machine(std::make_shared<StreamInput>(std::cin), std::make_shared<StreamOutput>(std::cout));
		machine.Execute(program);
		return 0;
	} catch (const SyntaxError& error) {
		std::cerr << "syntax error: " << error.what() << '\n';
		return 1;
	} catch (const RuntimeError& error) {
		std::cerr << "runtime error: " << error.what() << '\n';
		return 1;
	} catch (const std::exception& error) {
		std::cerr << "internal error: " << error.what() << '\n';
		return 3;
	}
}