#include <starforge/platform/fixed_step.hpp>
#include <starforge/render/render.hpp>

#include <catch2/catch_test_macros.hpp>

#include <cmath>
#include <limits>

TEST_CASE("fixed-step scheduler accumulates partial frames deterministically", "[backends]") {
    starforge::platform::FixedStepScheduler scheduler{0.01, 0.25, 8};

    const auto first = scheduler.advance(0.006);
    REQUIRE(first.steps == 0U);
    REQUIRE(first.interpolation_alpha == Catch::Approx(0.6));

    const auto second = scheduler.advance(0.006);
    REQUIRE(second.steps == 1U);
    REQUIRE(second.interpolation_alpha == Catch::Approx(0.2));
}

TEST_CASE("fixed-step scheduler clamps pathological frame deltas", "[backends]") {
    starforge::platform::FixedStepScheduler scheduler{0.01, 0.05, 3};

    const auto plan = scheduler.advance(std::numeric_limits<double>::infinity());
    REQUIRE(plan.steps == 0U);
    REQUIRE(plan.accepted_frame_seconds == 0.0);

    const auto clamped = scheduler.advance(1.0);
    REQUIRE(clamped.steps == 3U);
    REQUIRE(clamped.accepted_frame_seconds == Catch::Approx(0.05));
    REQUIRE(clamped.interpolation_alpha >= 0.0);
    REQUIRE(clamped.interpolation_alpha < 1.0);
}

TEST_CASE("renderer submit contracts reject zero-sized resources", "[backends]") {
    REQUIRE_FALSE(starforge::render::valid_frame({}));
    REQUIRE(starforge::render::valid_frame({.framebuffer_width = 1280, .framebuffer_height = 720}));

    REQUIRE(starforge::render::valid_texture_desc({.width = 1, .height = 1}));
    REQUIRE_FALSE(starforge::render::valid_texture_desc({.width = 0, .height = 1}));
    REQUIRE_FALSE(starforge::render::valid_texture_desc({.width = 1, .height = 0}));
}
