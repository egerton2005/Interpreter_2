#include "Compiler.hpp"

#include <memory>
#include <utility>

Program Compiler::Compile(const IStmt& program) {
	program_.clear();
	label_counter_ = 0;
	program.Accept(*this);
	return std::move(program_);
}

std::string Compiler::NewLabel() {
	return "L" + std::to_string(label_counter_++);
}

void Compiler::Emit(InstructionPtr instruction) {
	program_.push_back(std::move(instruction));
}

void Compiler::Visit(const Const& node) {
	Emit(std::make_shared<ConstantInstruction>(node.GetValue()));
}

void Compiler::Visit(const Var& node) {
	Emit(std::make_shared<LoadInstruction>(node.GetName()));
}

void Compiler::Visit(const BinOp& node) {
	node.GetLhs()->Accept(*this);
	node.GetRhs()->Accept(*this);
	Emit(std::make_shared<BinaryOperationInstruction>(node.GetOperator()));
}

void Compiler::Visit(const Skip&) {
}

void Compiler::Visit(const Assign& node) {
	node.GetValue()->Accept(*this);
	Emit(std::make_shared<StoreInstruction>(node.GetName()));
}

void Compiler::Visit(const Read& node) {
	Emit(std::make_shared<ReadInstruction>());
	Emit(std::make_shared<StoreInstruction>(node.GetName()));
}

void Compiler::Visit(const Write& node) {
	node.GetExpr()->Accept(*this);
	Emit(std::make_shared<WriteInstruction>());
}

void Compiler::Visit(const Block& node) {
	for (const IStmtPtr& statement : node.GetStatements()) {
		statement->Accept(*this);
	}
}

void Compiler::Visit(const If& node) {
	const std::string else_label = NewLabel();
	const std::string end_label = NewLabel();

	node.GetCond()->Accept(*this);
	Emit(std::make_shared<JumpZeroInstruction>(else_label));
	node.GetThenBranch()->Accept(*this);
	Emit(std::make_shared<JumpInstruction>(end_label));
	Emit(std::make_shared<LabelInstruction>(else_label));
	node.GetElseBranch()->Accept(*this);
	Emit(std::make_shared<LabelInstruction>(end_label));
}

void Compiler::Visit(const While& node) {
	const std::string start_label = NewLabel();
	const std::string end_label = NewLabel();

	Emit(std::make_shared<LabelInstruction>(start_label));
	node.GetCond()->Accept(*this);
	Emit(std::make_shared<JumpZeroInstruction>(end_label));
	node.GetBody()->Accept(*this);
	Emit(std::make_shared<JumpInstruction>(start_label));
	Emit(std::make_shared<LabelInstruction>(end_label));
}

void Compiler::Visit(const DoWhile& node) {
	const std::string start_label = NewLabel();

	Emit(std::make_shared<LabelInstruction>(start_label));
	node.GetBody()->Accept(*this);
	node.GetCond()->Accept(*this);
	Emit(std::make_shared<JumpNotZeroInstruction>(start_label));
}