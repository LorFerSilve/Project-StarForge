#pragma once

#include <source_location>
#include <string_view>

namespace starforge::diagnostics {

[[noreturn]] void fail_invariant(
    std::string_view expression,
    std::string_view message,
    std::source_location location = std::source_location::current()) noexcept;

}  // namespace starforge::diagnostics

#define SF_ASSERT(expression, message) \
    ((expression) ? static_cast<void>(0) \
                  : ::starforge::diagnostics::fail_invariant(#expression, (message)))
