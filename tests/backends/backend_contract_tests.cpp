#include <starforge/platform/fixed_step.hpp>
#include <starforge/platform/platform.hpp>
#include <starforge/render/render.hpp>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <limits>
#include <memory>

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

TEST_CASE("render snapshot is an immutable presentation handoff", "[backends]") {
    constexpr starforge::render::RenderSnapshot snapshot{42U, 7U, 3U, 99U};

    STATIC_REQUIRE(snapshot.tick_index() == 42U);
    STATIC_REQUIRE(snapshot.scene_generation() == 7U);
    STATIC_REQUIRE(snapshot.origin_epoch() == 3U);
    STATIC_REQUIRE(snapshot.state_revision() == 99U);
}

TEST_CASE("renderer submit contracts reject zero-sized resources", "[backends]") {
    REQUIRE_FALSE(starforge::render::valid_frame({}));
    REQUIRE(starforge::render::valid_frame({.framebuffer_width = 1280, .framebuffer_height = 720}));

    REQUIRE(starforge::render::valid_texture_desc({.width = 1, .height = 1}));
    REQUIRE_FALSE(starforge::render::valid_texture_desc({.width = 0, .height = 1}));
    REQUIRE_FALSE(starforge::render::valid_texture_desc({.width = 1, .height = 0}));
}

TEST_CASE("OpenGL backend opens a 4.6 core context and owns resource lifetime", "[backends][graphics]") {
    auto window = starforge::platform::create_window({
        .title = "StarForge IMP-3 backend smoke",
        .width = 64,
        .height = 64,
        .visible = false,
        .vsync = false,
    });

    auto renderer = starforge::render::create_renderer();
    const auto snapshot = std::make_shared<const starforge::render::RenderSnapshot>(42U, 7U, 3U, 99U);
    const auto size = window->framebuffer_size();
    REQUIRE(size.width > 0U);
    REQUIRE(size.height > 0U);

    const auto initialize_and_render = [&] {
        renderer->initialize(window->graphics_proc_resolver());

        const auto capabilities = renderer->capabilities();
        REQUIRE(capabilities.api_major == 4U);
        REQUIRE(capabilities.api_minor >= 6U);
        REQUIRE(capabilities.core_profile);
        REQUIRE(capabilities.debug_output);
        REQUIRE(capabilities.reversed_z);

        const auto initialized = renderer->diagnostics();
        REQUIRE(initialized.initialized);
        REQUIRE(initialized.debug_callback_active);
        REQUIRE(initialized.reversed_z_active);
        REQUIRE(initialized.live_gpu_objects == 3U);

        renderer->render({
            .framebuffer_width = size.width,
            .framebuffer_height = size.height,
            .snapshot = snapshot,
        });
        window->swap_buffers();

        const auto rendered = renderer->diagnostics();
        REQUIRE(rendered.last_frame_clean);
        REQUIRE(rendered.live_gpu_objects == 3U);
    };

    initialize_and_render();
    REQUIRE(renderer->diagnostics().rendered_frames == 1U);

    renderer->shutdown();
    const auto stopped_once = renderer->diagnostics();
    REQUIRE_FALSE(stopped_once.initialized);
    REQUIRE_FALSE(stopped_once.debug_callback_active);
    REQUIRE_FALSE(stopped_once.reversed_z_active);
    REQUIRE(stopped_once.live_gpu_objects == 0U);

    initialize_and_render();
    REQUIRE(renderer->diagnostics().rendered_frames == 2U);

    renderer->shutdown();
    const auto stopped_twice = renderer->diagnostics();
    REQUIRE_FALSE(stopped_twice.initialized);
    REQUIRE(stopped_twice.live_gpu_objects == 0U);
    REQUIRE(stopped_twice.rendered_frames == 2U);
}