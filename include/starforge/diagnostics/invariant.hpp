#pragma once

#include <source_location>
#include <stdexcept>
#include <string>
#include <string_view>

namespace starforge::diagnostics {

class InvariantViolation final : public std::logic_error {
public:
    explicit InvariantViolation(const std::string& message) : std::logic_error(message) {}
};

[[noreturn]] void invariant_failed(
    std::string_view expression,
    std::string_view message,
    std::source_location location = std::source_location::current());

inline void require_invariant(
    bool condition,
    std::string_view expression,
    std::string_view message,
    std::source_location location = std::source_location::current()) {
    if (!condition) {
        invariant_failed(expression, message, location);
    }
}

} // namespace starforge::diagnostics

#define SF_REQUIRE(expr, message) \
    ::starforge::diagnostics::require_invariant(static_cast<bool>(expr), #expr, message, std::source_location::current())
