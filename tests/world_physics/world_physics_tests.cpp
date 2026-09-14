#include <starforge/physics/jolt_world.hpp>
#include <starforge/player/character_motor.hpp>
#include <starforge/world/world.hpp>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>

namespace {

constexpr double tick_seconds = 1.0 / 60.0;

struct TestScene final {
    std::unique_ptr<starforge::physics::PhysicsWorld> world;
    starforge::physics::PhysicsBodyHandle character;
};

TestScene make_test_room(std::uint64_t generation) {
    auto world = starforge::physics::create_jolt_physics_world(generation);
    [[maybe_unused]] const auto floor = world->create_box({
        .body_class = starforge::physics::BodyClass::Static,
        .layer = starforge::physics::CollisionLayer::StaticWorld,
        .half_extents = {8.0, 0.5, 8.0},
        .position = {0.0, -0.5, 0.0},
    });
    const auto character = world->create_capsule({
        .body_class = starforge::physics::BodyClass::Character,
        .layer = starforge::physics::CollisionLayer::Character,
        .half_height = 0.8,
        .radius = 0.35,
        .position = {0.0, 3.0, 0.0},
    });
    return {.world = std::move(world), .character = character};
}

starforge::physics::Vec3 run_locomotion(std::uint64_t generation, std::size_t ticks, std::size_t batch_size) {
    auto scene = make_test_room(generation);
    starforge::player::CharacterMotor motor{{.max_speed_metres_per_second = 3.0}};
    starforge::player::InputSampleBuffer input;
    input.sample({.move_x = 1.0F, .move_z = 0.25F});

    std::size_t simulated = 0U;
    while (simulated < ticks) {
        const auto batch = (ticks - simulated < batch_size) ? ticks - simulated : batch_size;
        for (std::size_t index = 0U; index < batch; ++index) {
            motor.apply_fixed_tick(*scene.world, scene.character, input.latest());
            scene.world->step(tick_seconds);
            ++simulated;
        }
    }
    return scene.world->position(scene.character);
}

}  // namespace

TEST_CASE("runtime entity handles reject stale generations") {
    starforge::world::RuntimeEntityRegistry registry;
    const auto first = registry.create();
    REQUIRE(registry.contains(first));
    registry.destroy(first);
    REQUIRE_FALSE(registry.contains(first));

    const auto replacement = registry.create();
    REQUIRE(replacement.index == first.index);
    REQUIRE(replacement.generation != first.generation);
    REQUIRE(registry.contains(replacement));
}

TEST_CASE("required content keeps scene in hard streaming hold until ready") {
    constexpr std::array<starforge::world::RequiredContentKey, 2> required{11U, 22U};
    starforge::world::SceneInstance scene{
        7U,
        {.generation = 3U},
        required,
        starforge::physics::create_jolt_physics_world(7U),
    };

    REQUIRE(scene.state() == starforge::world::SceneState::Staging);
    REQUIRE(scene.hard_streaming_hold());
    scene.mark_content_resident(11U);
    REQUIRE(scene.hard_streaming_hold());
    scene.mark_content_resident(22U);
    REQUIRE(scene.state() == starforge::world::SceneState::Ready);
    REQUIRE_FALSE(scene.hard_streaming_hold());

    scene.activate();
    REQUIRE(scene.state() == starforge::world::SceneState::Active);
    scene.quiesce();
    scene.deactivate();
    REQUIRE(scene.state() == starforge::world::SceneState::Destroyed);
}

TEST_CASE("context positions are converted relative to the runtime origin") {
    const auto runtime = starforge::world::to_runtime_position(
        {1'000'010.0, -4.0, 2'000'005.0},
        {.x = 1'000'000.0, .y = -5.0, .z = 2'000'000.0, .generation = 9U});
    REQUIRE(runtime.x == Catch::Approx(10.0F));
    REQUIRE(runtime.y == Catch::Approx(1.0F));
    REQUIRE(runtime.z == Catch::Approx(5.0F));
}

TEST_CASE("Jolt character collides with the static test-room floor") {
    auto scene = make_test_room(21U);
    for (std::size_t tick = 0U; tick < 180U; ++tick) {
        scene.world->step(tick_seconds);
    }

    const auto position = scene.world->position(scene.character);
    REQUIRE(position.y > 1.0);
    REQUIRE(position.y < 1.3);
    REQUIRE(scene.world->body_count() == 2U);
}

TEST_CASE("authoritative locomotion checkpoints do not depend on render batching") {
    const auto one_tick_batches = run_locomotion(31U, 180U, 1U);
    const auto six_tick_batches = run_locomotion(32U, 180U, 6U);

    REQUIRE(one_tick_batches.x == Catch::Approx(six_tick_batches.x).margin(1.0e-5));
    REQUIRE(one_tick_batches.y == Catch::Approx(six_tick_batches.y).margin(1.0e-5));
    REQUIRE(one_tick_batches.z == Catch::Approx(six_tick_batches.z).margin(1.0e-5));
}

TEST_CASE("destroying a scene retires physics state before a replacement scene") {
    constexpr std::array<starforge::world::RequiredContentKey, 0> no_required_content{};
    {
        starforge::world::SceneInstance scene{
            41U,
            {.generation = 1U},
            no_required_content,
            starforge::physics::create_jolt_physics_world(41U),
        };
        REQUIRE(scene.state() == starforge::world::SceneState::Ready);
        scene.activate();
        [[maybe_unused]] const auto body = scene.physics_world().create_box({
            .body_class = starforge::physics::BodyClass::Dynamic,
            .layer = starforge::physics::CollisionLayer::DynamicWorld,
            .position = {0.0, 2.0, 0.0},
        });
        REQUIRE(scene.physics_world().body_count() == 1U);
    }

    auto replacement = starforge::physics::create_jolt_physics_world(42U);
    REQUIRE(replacement->body_count() == 0U);
}
