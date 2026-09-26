#pragma once

#include <iosfwd>

#include "parser/IParser.hpp"
#include "runtime/IMachine.hpp"

class Interpreter {
public:
    Interpreter(ParserPtr parser, MachinePtr machine);
    void Run(std::istream& program) const;

private:
    ParserPtr parser_;
    MachinePtr machine_;
};
