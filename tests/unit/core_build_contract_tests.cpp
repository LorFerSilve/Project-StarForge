#include "starforge/core/build_contract.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Core.BuildContract.TA16BaselineIsLocked", "[unit]") {
    REQUIRE(starforge::core::build_contract_name() == "TA16-V1");
}

TEST_CASE("Core.BuildContract.AuthoritativeTickRateIs60Hz", "[unit]") {
    REQUIRE(starforge::core::kAuthoritativeSimulationHz == 60U);
}

TEST_CASE("Core.Language.Cpp23BaselineIsLocked", "[unit]") {
    STATIC_REQUIRE(starforge::core::kRequiredCppStandard == 23U);
}
