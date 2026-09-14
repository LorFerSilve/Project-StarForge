#include <starforge/physics/jolt_world.hpp>
#include <starforge/platform/fixed_step.hpp>
#include <starforge/platform/platform.hpp>
#include <starforge/player/character_motor.hpp>
#include <starforge/render/render.hpp>
#include <starforge/transactions/coordinator.hpp>
#include <starforge/vertical_slice/horizon_test_cell.hpp>
#include <starforge/world/world.hpp>

#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <system_error>
#include <utility>
#include <vector>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace {
constexpr const char* save_path = "starforge_horizon_v0.sfgsave";
constexpr const char* temporary_save_path = "starforge_horizon_v0.sfgsave.tmp";

std::optional<std::vector<std::byte>> read_save_file() {
    std::error_code exists_error;
    const bool exists = std::filesystem::exists(save_path, exists_error);
    if (exists_error) {
        throw std::runtime_error{"failed to inspect Horizon V0 save file"};
    }
    if (!exists) {
        return std::nullopt;
    }

    std::ifstream file{save_path, std::ios::binary | std::ios::ate};
    if (!file) {
        throw std::runtime_error{"failed to open existing Horizon V0 save file"};
    }
    const auto size = file.tellg();
    if (size <= 0) {
        throw std::runtime_error{"existing Horizon V0 save file is empty or unreadable"};
    }
    std::vector<std::byte> bytes(static_cast<std::size_t>(size));
    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
    if (!file) {
        throw std::runtime_error{"failed to read existing Horizon V0 save file"};
    }
    return bytes;
}

void replace_save_atomically() {
#ifdef _WIN32
    if (MoveFileExA(temporary_save_path, save_path, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) == 0) {
        static_cast<void>(std::remove(temporary_save_path));
        throw std::runtime_error{"failed to atomically replace Horizon V0 save file"};
    }
#else
    if (std::rename(temporary_save_path, save_path) != 0) {
        static_cast<void>(std::remove(temporary_save_path));
        throw std::runtime_error{"failed to atomically replace Horizon V0 save file"};
    }
#endif
}

void write_save_file(const std::vector<std::byte>& bytes) {
    {
        std::ofstream file{temporary_save_path, std::ios::binary | std::ios::trunc};
        if (!file) {
            throw std::runtime_error{"failed to open Horizon V0 temporary save file"};
        }
        file.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
        file.flush();
        if (!file) {
            static_cast<void>(std::remove(temporary_save_path));
            throw std::runtime_error{"failed to write Horizon V0 temporary save file"};
        }
    }
    replace_save_atomically();
}

starforge::physics::PhysicsBodyHandle create_item_body(
    starforge::physics::PhysicsWorld& physics_world,
    const starforge::vertical_slice::WorldItemPhysicalState& state) {
    return physics_world.create_box({
        .body_class = starforge::physics::BodyClass::Dynamic,
        .layer = starforge::physics::CollisionLayer::DynamicWorld,
        .half_extents = {0.25, 0.25, 0.25},
        .position = state.position,
        .linear_velocity = state.linear_velocity,
    });
}

void sync_item_physical_state(
    starforge::vertical_slice::HorizonTestCell& test_cell,
    const starforge::physics::PhysicsWorld& physics_world,
    const std::optional<starforge::physics::PhysicsBodyHandle>& item_body) {
    if (!item_body.has_value() || !physics_world.contains(*item_body)) {
        return;
    }
    test_cell.update_world_physical_state(
        physics_world.position(*item_body),
        physics_world.linear_velocity(*item_body));
}
}  // namespace

int main() {
    try {
        auto window = starforge::platform::create_window({
            .title = "Project StarForge - IMP-5 Horizon Test Cell",
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

        starforge::transactions::TransactionCoordinator transaction_coordinator;
        auto test_cell = std::make_unique<starforge::vertical_slice::HorizonTestCell>();
        const auto saved_bytes = read_save_file();
        if (saved_bytes.has_value()) {
            auto loaded = starforge::vertical_slice::HorizonTestCell::load(*saved_bytes);
            if (!loaded) {
                throw std::runtime_error{"existing Horizon V0 save failed validation"};
            }
            test_cell = std::make_unique<starforge::vertical_slice::HorizonTestCell>(std::move(loaded).value());
            std::cout << "Loaded Horizon V0 persistent state\n";
        }

        const auto item_runtime = test_cell->enter_context(scene.entities());
        std::optional<starforge::physics::PhysicsBodyHandle> item_body;
        if (item_runtime.valid()) {
            item_body = create_item_body(scene.physics_world(), test_cell->read_model().world_physical_state);
            std::cout << "Horizon test item is in the room. Press E to transfer it to inventory.\n";
        } else {
            std::cout << "Horizon test item is in player inventory. Press E to return it to the room.\n";
        }

        starforge::player::InputSampleBuffer input_buffer;
        const starforge::player::CharacterMotor character_motor{};
        starforge::platform::FixedStepScheduler scheduler{};
        auto previous = std::chrono::steady_clock::now();
        std::uint64_t tick_index = 0U;
        bool previous_interact = false;
        bool interaction_latched = false;

        while (!window->should_close()) {
            window->poll_events();
            const auto input = window->input();
            if (input.escape) {
                window->request_close();
            }

            if (input.e && !previous_interact) {
                interaction_latched = true;
            }
            previous_interact = input.e;

            input_buffer.sample({
                .move_x = static_cast<float>(input.d) - static_cast<float>(input.a),
                .move_z = static_cast<float>(input.s) - static_cast<float>(input.w),
            });

            const auto now = std::chrono::steady_clock::now();
            const std::chrono::duration<double> elapsed = now - previous;
            previous = now;

            const auto step_plan = scheduler.advance(elapsed.count());
            for (std::uint32_t step = 0; step < step_plan.steps; ++step) {
                bool save_after_step = false;
                if (interaction_latched) {
                    const auto current_owner = test_cell->read_model().owner;
                    if (current_owner == starforge::vertical_slice::ItemOwner::WorldContainer) {
                        sync_item_physical_state(*test_cell, scene.physics_world(), item_body);
                    }
                    const auto destination = current_owner == starforge::vertical_slice::ItemOwner::WorldContainer
                        ? starforge::vertical_slice::ItemOwner::PlayerInventory
                        : starforge::vertical_slice::ItemOwner::WorldContainer;
                    const auto transferred = test_cell->transfer(destination, transaction_coordinator, scene.entities());
                    if (transferred) {
                        if (destination == starforge::vertical_slice::ItemOwner::PlayerInventory) {
                            if (item_body.has_value() && scene.physics_world().contains(*item_body)) {
                                scene.physics_world().destroy_body(*item_body);
                            }
                            item_body.reset();
                            std::cout << "Committed item transfer to player inventory.\n";
                        } else {
                            item_body = create_item_body(
                                scene.physics_world(), test_cell->read_model().world_physical_state);
                            std::cout << "Committed item transfer back to the Horizon room.\n";
                        }
                        save_after_step = true;
                    }
                    interaction_latched = false;
                }

                character_motor.apply_fixed_tick(scene.physics_world(), character, input_buffer.latest());
                scene.physics_world().step(1.0 / 60.0);
                ++tick_index;
                sync_item_physical_state(*test_cell, scene.physics_world(), item_body);

                if (save_after_step) {
                    const auto save = test_cell->save(tick_index, transaction_coordinator);
                    if (save) {
                        write_save_file(save.value());
                    }
                }
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

        sync_item_physical_state(*test_cell, scene.physics_world(), item_body);
        const auto final_save = test_cell->save(tick_index, transaction_coordinator);
        if (final_save) {
            write_save_file(final_save.value());
        }
        test_cell->leave_context(scene.entities());
        scene.quiesce();
        scene.deactivate();
        renderer->shutdown();
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "starforge bootstrap failed: " << error.what() << '\n';
        return 1;
    }
}