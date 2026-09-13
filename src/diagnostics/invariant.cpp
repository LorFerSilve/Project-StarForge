#include "starforge/diagnostics/invariant.hpp"

#include <sstream>

namespace starforge::diagnostics {

void invariant_failed(
    std::string_view expression,
    std::string_view message,
    std::source_location location) {
    std::ostringstream stream;
    stream << "Invariant failed: " << expression;
    if (!message.empty()) {
        stream << " (" << message << ')';
    }
    stream << " at " << location.file_name() << ':' << location.line();
    throw InvariantViolation{stream.str()};
}

} // namespace starforge::diagnostics
