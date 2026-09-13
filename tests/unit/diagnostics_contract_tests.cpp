#include "starforge/diagnostics/log.hpp"

#include <catch2/catch_test_macros.hpp>

#include <sstream>

TEST_CASE("Diagnostics.Log.StreamLoggerProducesTypedContext", "[unit][diagnostics]") {
    std::ostringstream output;
    starforge::diagnostics::StreamLogger logger{output};
    logger.write({
        starforge::diagnostics::LogSeverity::Warning,
        starforge::core::DomainId::Simulation,
        starforge::core::SimulationTick{42U},
        "backlog detected",
    });

    REQUIRE(output.str() == "[warning] [simulation] [tick=42] backlog detected\n");
}

TEST_CASE("Diagnostics.Log.NullLoggerIsSideEffectFree", "[unit][diagnostics]") {
    starforge::diagnostics::NullLogger logger;
    REQUIRE_NOTHROW(logger.write({
        starforge::diagnostics::LogSeverity::Info,
        starforge::core::DomainId::Core,
        std::nullopt,
        "ignored",
    }));
}
