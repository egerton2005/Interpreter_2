#pragma once

#include <stdexcept>
#include <string>

class InterpreterError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class SyntaxError : public InterpreterError {
public:
    using InterpreterError::InterpreterError;
};

class RuntimeError : public InterpreterError {
public:
    using InterpreterError::InterpreterError;
};

class DivisionByZero : public RuntimeError {
public:
    DivisionByZero()
    : RuntimeError("division by zero")
    { }
};

class StackUnderflow : public RuntimeError {
public:
    StackUnderflow()
    : RuntimeError("stack underflow")
    { }
};

class InvalidInput : public RuntimeError {
public:
    InvalidInput()
    : RuntimeError("invalid or exhausted input")
    { }
};

class UndefinedVariable : public RuntimeError {
public:
    explicit UndefinedVariable(const std::string& name)
    : RuntimeError("undefined variable: " + name)
    { }
};

class UndefinedLabel : public RuntimeError {
public:
    explicit UndefinedLabel(const std::string& name)
    : RuntimeError("undefined label: " + name)
    { }
};

class DuplicateLabel : public RuntimeError {
public:
    explicit DuplicateLabel(const std::string& name)
    : RuntimeError("duplicate label: " + name)
    { }
};
