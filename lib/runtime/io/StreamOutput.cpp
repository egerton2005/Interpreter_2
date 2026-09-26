#include "runtime/io/StreamOutput.hpp"

StreamOutput::StreamOutput(std::ostream& output)
: output_(output)
{ }

void StreamOutput::Write(const Value& value) {
    output_ << value << '\n';
}
