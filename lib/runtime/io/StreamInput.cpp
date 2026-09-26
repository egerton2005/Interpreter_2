#include "runtime/io/StreamInput.hpp"

#include "error/Errors.hpp"

StreamInput::StreamInput(std::istream& input)
: input_(input)
{ }

Value StreamInput::Read() {
    Value value;
    if (!(input_ >> value)) {
        throw InvalidInput();
    }
    return value;
}
