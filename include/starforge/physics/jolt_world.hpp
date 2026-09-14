#pragma once

#include <starforge/physics/physics.hpp>

#include <cstdint>
#include <memory>

namespace starforge::physics {

[[nodiscard]] std::unique_ptr<PhysicsWorld> create_jolt_physics_world(std::uint64_t scene_generation);

}  // namespace starforge::physics
