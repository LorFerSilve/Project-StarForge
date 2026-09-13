#pragma once

#include "starforge/core/domain.hpp"
#include "starforge/core/simulation_time.hpp"

#include <iosfwd>
#include <optional>
#include <string_view>

namespace starforge::diagnostics {

enum class LogSeverity {
    Trace,
    Debug,
    Info,
    Warning,
    Error,
    Critical,
};

struct LogRecord final {
    LogSeverity severity{LogSeverity::Info};
    starforge::core::DomainId domain{starforge::core::DomainId::Core};
    std::optional<starforge::core::SimulationTick> simulation_tick{};
    std::string_view message{};
};

class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void write(const LogRecord& record) noexcept = 0;
};

class NullLogger final : public ILogger {
public:
    void write(const LogRecord&) noexcept override {}
};

class StreamLogger final : public ILogger {
public:
    explicit StreamLogger(std::ostream& stream) noexcept;
    void write(const LogRecord& record) noexcept override;

private:
    std::ostream* stream_;
};

[[nodiscard]] std::string_view to_string(LogSeverity severity) noexcept;
[[nodiscard]] std::string_view to_string(starforge::core::DomainId domain) noexcept;

}  // namespace starforge::diagnostics
