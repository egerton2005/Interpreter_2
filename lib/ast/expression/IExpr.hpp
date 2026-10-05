#pragma once

#include <memory>

class IExprVisitor;

class IExpr {
    public:
    IExpr(const IExpr&) = delete;
    IExpr& operator=(const IExpr&) = delete;
    
    virtual void Accept(IExprVisitor& visitor) const = 0;
    
    virtual ~IExpr() = default;
    
    protected:
    IExpr() = default;
};

using IExprPtr = std::shared_ptr<IExpr>;