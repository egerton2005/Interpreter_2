#pragma once

#include <cstdint>
#include <iosfwd>

class Value {
public:
    Value() noexcept;
    explicit Value(std::int32_t value) noexcept;
    explicit Value(bool value) noexcept;

    std::int32_t Raw() const noexcept;
    explicit operator bool() const noexcept;

    Value operator+(const Value& other) const;
    Value operator-(const Value& other) const;
    Value operator*(const Value& other) const;
    Value operator/(const Value& other) const;
    Value operator%(const Value& other) const;

    bool operator==(const Value& other) const noexcept;
    bool operator!=(const Value& other) const noexcept;
    bool operator<(const Value& other) const noexcept;
    bool operator>(const Value& other) const noexcept;
    bool operator<=(const Value& other) const noexcept;
    bool operator>=(const Value& other) const noexcept;

    friend std::ostream& operator<<(std::ostream& output, const Value& value);
    friend std::istream& operator>>(std::istream& input, Value& value);

private:
    std::int32_t value_ = 0;
};
