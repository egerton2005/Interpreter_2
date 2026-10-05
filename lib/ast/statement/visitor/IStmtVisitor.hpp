#pragma once

class Skip;
class Assign;
class Read;
class Write;
class If;
class While;
class DoWhile;
class Block;

class IStmtVisitor {
public:
    IStmtVisitor(const IStmtVisitor&) = delete;
    IStmtVisitor& operator=(const IStmtVisitor&) = delete;

    virtual void Visit(const Skip& node) = 0;
    virtual void Visit(const Assign& node) = 0;
    virtual void Visit(const Read& node) = 0;
    virtual void Visit(const Write& node) = 0;
    virtual void Visit(const If& node) = 0;
    virtual void Visit(const While& node) = 0;
    virtual void Visit(const DoWhile& node) = 0;
    virtual void Visit(const Block& node) = 0;

    virtual ~IStmtVisitor() = default;

protected:
    IStmtVisitor() = default;
};