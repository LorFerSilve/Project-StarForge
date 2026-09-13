#include <starforge/platform/fixed_step.hpp>
#include <starforge/platform/platform.hpp>
#include <starforge/render/render.hpp>

#include <chrono>
#include <cstdint>
#include <exception>
#include <iostream>

int main() {
    try {
        auto window = starforge::platform::create_window({
            .title = "Project StarForge - IMP-3 Render Bootstrap",
            .width = 1280,
            .height = 720,
            .visible = true,
            .vsync = true,
        });

        auto renderer = starforge::render::create_renderer();
        renderer->initialize(window->graphics_proc_resolver());

        starforge::platform::FixedStepScheduler scheduler{};
        auto previous = std::chrono::steady_clock::now();

        while (!window->should_close()) {
            window->poll_events();
            if (window->input().escape) {
                window->request_close();
            }

            const auto now = std::chrono::steady_clock::now();
            const std::chrono::duration<double> elapsed = now - previous;
            previous = now;

            const auto step_plan = scheduler.advance(elapsed.count());
            for (std::uint32_t step = 0; step < step_plan.steps; ++step) {
                // IMP-3 establishes the deterministic fixed-step boundary only.
                // Runtime scene simulation is introduced by the next roadmap slice.
            }

            const auto size = window->framebuffer_size();
            renderer->render({
                .framebuffer_width = size.width,
                .framebuffer_height = size.height,
            });
            window->swap_buffers();
        }

        renderer->shutdown();
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "starforge bootstrap failed: " << error.what() << '\n';
        return 1;
    }
}
