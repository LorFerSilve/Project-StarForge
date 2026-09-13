#include "starforge/diagnostics/log.hpp"

#include <ostream>

namespace starforge::diagnostics {

StreamLogger::StreamLogger(std::ostream& stream) noexcept : stream_(&stream) {}

void StreamLogger::write(const LogRecord& record) noexcept {
    try {
        *stream_ << '[' << to_string(record.severity) << "] [" << to_string(record.domain) << ']';
        if (record.simulation_tick.has_value()) {
            *stream_ << " [tick=" << record.simulation_tick->raw() << ']';
        }
        *stream_ << ' ' << record.message << '\n';
    } catch (...) {
        // Diagnostics must never become a second failure path for simulation code.
    }
}

std::string_view to_string(LogSeverity severity) noexcept {
    switch (severity) {
    case LogSeverity::Trace: return "trace";
    case LogSeverity::Debug: return "debug";
    case LogSeverity::Info: return "info";
    case LogSeverity::Warning: return "warning";
    case LogSeverity::Error: return "error";
    case LogSeverity::Critical: return "critical";
    }
    return "unknown";
}

std::string_view to_string(starforge::core::DomainId domain) noexcept {
    using starforge::core::DomainId;
    switch (domain) {
    case DomainId::Core: return "core";
    case DomainId::Diagnostics: return "diagnostics";
    case DomainId::Simulation: return "simulation";
    case DomainId::Transactions: return "transactions";
    case DomainId::ReadModels: return "read-models";
    case DomainId::Content: return "content";
    case DomainId::World: return "world";
    case DomainId::Player: return "player";
    case DomainId::Combat: return "combat";
    case DomainId::Crew: return "crew";
    case DomainId::Robots: return "robots";
    case DomainId::Spacecraft: return "spacecraft";
    case DomainId::Station: return "station";
    case DomainId::Economy: return "economy";
    case DomainId::Progression: return "progression";
    case DomainId::Ai: return "ai";
    case DomainId::Missions: return "missions";
    case DomainId::Strategic: return "strategic";
    }
    return "unknown";
}

}  // namespace starforge::diagnostics
