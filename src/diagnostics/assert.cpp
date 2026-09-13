#include "starforge/diagnostics/assert.hpp"

#include <cstdio>
#include <cstdlib>

namespace starforge::diagnostics {

[[noreturn]] void fail_invariant(
    std::string_view expression,
    std::string_view message,
    std::source_location location) noexcept {
    std::fprintf(
        stderr,
        "StarForge invariant failure: %.*s (%.*s) at %s:%u in %s\n",
        static_cast<int>(expression.size()),
        expression.data(),
        static_cast<int>(message.size()),
        message.data(),
        location.file_name(),
        location.line(),
        location.function_name());
    std::abort();
}

}  // namespace starforge::diagnostics
