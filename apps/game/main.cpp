#include <starforge/physics/jolt_world.hpp>
#include <starforge/platform/fixed_step.hpp>
#include <starforge/platform/platform.hpp>
#include <starforge/player/character_motor.hpp>
#include <starforge/render/render.hpp>
#include <starforge/world/world.hpp>

#include <array>
#include <chrono>
#include <cstdint>
#include <exception>
#include <iostream>
#include <memory>

int main() {
    try {
        auto window = starforge::platform::create_window({
            .title = "Project StarForge - IMP-4 Active World",
            .width = 1280,
            .height = 720,
            .visible = true,
            .vsync = true,
        });

        auto renderer = starforge::render::create_renderer();
        renderer->initialize(window->graphics_proc_resolver());

        constexpr std::uint64_t scene_generation = 1U;
        constexpr std::array<starforge::world::RequiredContentKey, 0> no_required_content{};
        starforge::world::SceneInstance scene{
            scene_generation,
            {.generation = 1U},
            no_required_content,
            starforge::physics::create_jolt_physics_world(scene_generation),
        };
        scene.activate();

        [[maybe_unused]] const auto floor = scene.physics_world().create_box({
            .body_class = starforge::physics::BodyClass::Static,
            .layer = starforge::physics::CollisionLayer::StaticWorld,
            .half_extents = {8.0, 0.5, 8.0},
            .position = {0.0, -0.5, 0.0},
        });
        const auto character = scene.physics_world().create_capsule({
            .body_class = starforge::physics::BodyClass::Character,
            .layer = starforge::physics::CollisionLayer::Character,
            .half_height = 0.8,
            .radius = 0.35,
            .position = {0.0, 1.2, 0.0},
        });

        starforge::player::InputSampleBuffer input_buffer;
        const starforge::player::CharacterMotor character_motor{};
        starforge::platform::FixedStepScheduler scheduler{};
        auto previous = std::chrono::steady_clock::now();
        std::uint64_t tick_index = 0U;

        while (!window->should_close()) {
            window->poll_events();
            const auto input = window->input();
            if (input.escape) {
                window->request_close();
            }

            input_buffer.sample({
                .move_x = static_cast<float>(input.d) - static_cast<float>(input.a),
                .move_z = static_cast<float>(input.s) - static_cast<float>(input.w),
            });

            const auto now = std::chrono::steady_clock::now();
            const std::chrono::duration<double> elapsed = now - previous;
            previous = now;

            const auto step_plan = scheduler.advance(elapsed.count());
            for (std::uint32_t step = 0; step < step_plan.steps; ++step) {
                character_motor.apply_fixed_tick(scene.physics_world(), character, input_buffer.latest());
                scene.physics_world().step(1.0 / 60.0);
                ++tick_index;
            }

            const auto snapshot = std::make_shared<const starforge::render::RenderSnapshot>(
                tick_index,
                scene.scene_generation(),
                scene.origin().generation,
                tick_index);
            const auto size = window->framebuffer_size();
            renderer->render({
                .framebuffer_width = size.width,
                .framebuffer_height = size.height,
                .snapshot = snapshot,
            });
            window->swap_buffers();
        }

        scene.quiesce();
        scene.deactivate();
        renderer->shutdown();
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "starforge bootstrap failed: " << error.what() << '\n';
        return 1;
    }
}