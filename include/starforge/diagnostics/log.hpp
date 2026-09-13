#pragma once

#include <cstdint>
#include <string_view>

namespace starforge::diagnostics {

enum class Severity : std::uint8_t { Trace, Debug, Info, Warning, Error, Critical };

struct LogRecord final {
    Severity severity{Severity::Info};
    std::string_view category{};
    std::string_view message{};
    std::uint64_t simulation_tick{0};
};

class LogSink {
public:
    virtual ~LogSink() = default;
    virtual void write(const LogRecord& record) = 0;
};

class NullLogSink final : public LogSink {
public:
    void write(const LogRecord&) override {}
};

[[nodiscard]] std::string_view severity_name(Severity severity) noexcept;

} // namespace starforge::diagnostics
