#include "value/Value.hpp"

#include <istream>
#include <ostream>

#include "error/Errors.hpp"

Value::Value() noexcept = default;

Value::Value(std::int32_t value) noexcept
: value_(value)
{ }

Value::Value(bool value) noexcept
: value_(value ? 1 : 0)
{ }

std::int32_t Value::Raw() const noexcept {
    return value_;
}

Value::operator bool() const noexcept {
    return value_ != 0;
}

Value Value::operator+(const Value& other) const {
    return Value(value_ + other.value_);
}

Value Value::operator-(const Value& other) const {
    return Value(value_ - other.value_);
}

Value Value::operator*(const Value& other) const {
    return Value(value_ * other.value_);
}

Value Value::operator/(const Value& other) const {
    if (other.value_ == 0) {
        throw DivisionByZero();
    }
    return Value(value_ / other.value_);
}

Value Value::operator%(const Value& other) const {
    if (other.value_ == 0) {
        throw DivisionByZero();
    }
    return Value(value_ % other.value_);
}

bool Value::operator==(const Value& other) const noexcept {
    return value_ == other.value_;
}

bool Value::operator!=(const Value& other) const noexcept {
    return !(*this == other);
}

bool Value::operator<(const Value& other) const noexcept {
    return value_ < other.value_;
}

bool Value::operator>(const Value& other) const noexcept {
    return other < *this;
}

bool Value::operator<=(const Value& other) const noexcept {
    return !(*this > other);
}

bool Value::operator>=(const Value& other) const noexcept {
    return !(*this < other);
}

std::ostream& operator<<(std::ostream& output, const Value& value) {
    return output << value.value_;
}

std::istream& operator>>(std::istream& input, Value& value) {
    return input >> value.value_;
}
