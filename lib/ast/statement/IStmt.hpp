#pragma once

#include <memory>

class IStmtVisitor;

class IStmt {
    public:
    IStmt(const IStmt&) = delete;
    IStmt& operator=(const IStmt&) = delete;
    
    virtual void Accept(IStmtVisitor& visitor) const = 0;
    
    virtual ~IStmt() = default;
    
    protected:
    IStmt() = default;
};

using IStmtPtr = std::shared_ptr<IStmt>;