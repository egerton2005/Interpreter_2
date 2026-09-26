#include "runtime/Interpreter.hpp"

#include <utility>

Interpreter::Interpreter(ParserPtr parser, MachinePtr machine)
: parser_(std::move(parser))
, machine_(std::move(machine))
{ }

void Interpreter::Run(std::istream& program) const {
    machine_->Execute(parser_->Parse(program));
}
