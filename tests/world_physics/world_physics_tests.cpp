#include <starforge/physics/jolt_world.hpp>
#include <starforge/player/character_motor.hpp>
#include <starforge/world/world.hpp>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>

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

std::unique_ptr<starforge::world::SceneInstance> make_ready_scene(std::uint64_t generation,
                                                                  std::uint64_t origin_generation) {
    constexpr std::array<starforge::world::RequiredContentKey, 0> no_required_content{};
    return std::make_unique<starforge::world::SceneInstance>(
        generation,
        starforge::world::ContextOrigin{.generation = origin_generation},
        no_required_content,
        starforge::physics::create_jolt_physics_world(generation));
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
    starforge::world::RuntimeEntityRegistry registry{61U};
    const auto first = registry.create();
    REQUIRE(registry.contains(first));
    registry.destroy(first);
    REQUIRE_FALSE(registry.contains(first));

    const auto replacement = registry.create();
    REQUIRE(replacement.scene_generation == first.scene_generation);
    REQUIRE(replacement.index == first.index);
    REQUIRE(replacement.generation != first.generation);
    REQUIRE(registry.contains(replacement));
}

TEST_CASE("runtime entity handles are scoped to their scene generation") {
    starforge::world::RuntimeEntityRegistry first_registry{62U};
    const auto old_handle = first_registry.create();

    starforge::world::RuntimeEntityRegistry replacement_registry{63U};
    const auto replacement_handle = replacement_registry.create();

    REQUIRE(old_handle.index == replacement_handle.index);
    REQUIRE(old_handle.generation == replacement_handle.generation);
    REQUIRE(old_handle.scene_generation != replacement_handle.scene_generation);
    REQUIRE_FALSE(replacement_registry.contains(old_handle));
    REQUIRE_THROWS_AS(replacement_registry.destroy(old_handle), std::invalid_argument);
    REQUIRE(replacement_registry.contains(replacement_handle));
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

TEST_CASE("active local context replaces rather than overlaps authoritative scenes") {
    starforge::world::ActiveLocalContext context;
    context.activate(
        {.kind = starforge::world::ActiveLocalContextKind::Horizon,
         .owner_key = 101U,
         .scene_generation = 51U,
         .origin_generation = 5U,
         .scene_profile_key = 1001U},
        make_ready_scene(51U, 5U));

    REQUIRE(context.has_active_scene());
    REQUIRE(context.scene().scene_generation() == 51U);

    context.activate(
        {.kind = starforge::world::ActiveLocalContextKind::Mission,
         .owner_key = 202U,
         .scene_generation = 52U,
         .origin_generation = 6U,
         .scene_profile_key = 1002U},
        make_ready_scene(52U, 6U));

    REQUIRE(context.has_active_scene());
    REQUIRE(context.scene().scene_generation() == 52U);
    REQUIRE(context.descriptor()->owner_key == 202U);
    context.deactivate();
    REQUIRE_FALSE(context.has_active_scene());
    REQUIRE_FALSE(context.descriptor().has_value());
}

TEST_CASE("context positions are converted relative to the runtime origin") {
    const auto runtime = starforge::world::to_runtime_position(
        {1'000'010.0, -4.0, 2'000'005.0},
        {.x = 1'000'000.0, .y = -5.0, .z = 2'000'000.0, .generation = 9U});
    REQUIRE(runtime.x == Catch::Approx(10.0F));
    REQUIRE(runtime.y == Catch::Approx(1.0F));
    REQUIRE(runtime.z == Catch::Approx(5.0F));
}

TEST_CASE("incompatible physics body classes and collision layers are rejected") {
    auto world = starforge::physics::create_jolt_physics_world(19U);

    REQUIRE_THROWS_AS(
        world->create_box({
            .body_class = starforge::physics::BodyClass::Dynamic,
            .layer = starforge::physics::CollisionLayer::StaticWorld,
        }),
        std::invalid_argument);
    REQUIRE_THROWS_AS(
        world->create_capsule({
            .body_class = starforge::physics::BodyClass::Character,
            .layer = starforge::physics::CollisionLayer::DynamicWorld,
        }),
        std::invalid_argument);
    REQUIRE(world->body_count() == 0U);
}

TEST_CASE("Jolt collision queries expose project-owned handles") {
    auto world = starforge::physics::create_jolt_physics_world(20U);
    const auto floor = world->create_box({
        .body_class = starforge::physics::BodyClass::Static,
        .layer = starforge::physics::CollisionLayer::StaticWorld,
        .half_extents = {4.0, 0.5, 4.0},
        .position = {0.0, -0.5, 0.0},
    });

    const auto hit = world->raycast({0.0, 5.0, 0.0}, {0.0, -10.0, 0.0});
    REQUIRE(hit.has_value());
    REQUIRE(hit->body == floor);
    REQUIRE(hit->fraction == Catch::Approx(0.5).margin(1.0e-5));
    REQUIRE(hit->position.y == Catch::Approx(0.0).margin(1.0e-5));
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
