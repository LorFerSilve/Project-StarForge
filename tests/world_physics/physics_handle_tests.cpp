#include <starforge/physics/jolt_world.hpp>

#include <catch2/catch_test_macros.hpp>

#include <stdexcept>

TEST_CASE("physics handles are scoped to their scene generation") {
    auto first_world = starforge::physics::create_jolt_physics_world(71U);
    const auto old_handle = first_world->create_box({
        .body_class = starforge::physics::BodyClass::Dynamic,
        .layer = starforge::physics::CollisionLayer::DynamicWorld,
        .position = {0.0, 2.0, 0.0},
    });

    auto replacement_world = starforge::physics::create_jolt_physics_world(72U);
    const auto replacement_handle = replacement_world->create_box({
        .body_class = starforge::physics::BodyClass::Dynamic,
        .layer = starforge::physics::CollisionLayer::DynamicWorld,
        .position = {0.0, 2.0, 0.0},
    });

    REQUIRE(old_handle.index == replacement_handle.index);
    REQUIRE(old_handle.generation == replacement_handle.generation);
    REQUIRE(old_handle.scene_generation != replacement_handle.scene_generation);
    REQUIRE_FALSE(replacement_world->contains(old_handle));
    REQUIRE_THROWS_AS(replacement_world->position(old_handle), std::invalid_argument);
    REQUIRE(replacement_world->contains(replacement_handle));
}

TEST_CASE("physics handle slots are reused with a new generation") {
    auto world = starforge::physics::create_jolt_physics_world(73U);
    const auto first = world->create_box({
        .body_class = starforge::physics::BodyClass::Dynamic,
        .layer = starforge::physics::CollisionLayer::DynamicWorld,
        .position = {0.0, 2.0, 0.0},
    });

    world->destroy_body(first);
    REQUIRE_FALSE(world->contains(first));
    REQUIRE(world->body_count() == 0U);

    const auto replacement = world->create_box({
        .body_class = starforge::physics::BodyClass::Dynamic,
        .layer = starforge::physics::CollisionLayer::DynamicWorld,
        .position = {0.0, 3.0, 0.0},
    });

    REQUIRE(replacement.scene_generation == first.scene_generation);
    REQUIRE(replacement.index == first.index);
    REQUIRE(replacement.generation != first.generation);
    REQUIRE_FALSE(world->contains(first));
    REQUIRE(world->contains(replacement));
    REQUIRE(world->body_count() == 1U);
}
