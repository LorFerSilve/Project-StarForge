#include "starforge/diagnostics/log.hpp"

namespace starforge::diagnostics {

std::string_view severity_name(Severity severity) noexcept {
    switch (severity) {
    case Severity::Trace: return "trace";
    case Severity::Debug: return "debug";
    case Severity::Info: return "info";
    case Severity::Warning: return "warning";
    case Severity::Error: return "error";
    case Severity::Critical: return "critical";
    }
    return "unknown";
}

} // namespace starforge::diagnostics
