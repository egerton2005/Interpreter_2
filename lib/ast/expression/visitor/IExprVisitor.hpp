#pragma once

class Var;
class Const;
class BinOp;

class IExprVisitor {
public:
    IExprVisitor(const IExprVisitor&) = delete;
    IExprVisitor& operator=(const IExprVisitor&) = delete;

    virtual void Visit(const Var& node) = 0;
    virtual void Visit(const Const& node) = 0;
    virtual void Visit(const BinOp& node) = 0;

    virtual ~IExprVisitor() = default;

protected:
    IExprVisitor() = default;
};