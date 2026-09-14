#pragma once

#include <starforge/physics/physics.hpp>

namespace starforge::player {

struct LocomotionInput final {
    float move_x{0.0F};
    float move_z{0.0F};
};

class InputSampleBuffer final {
public:
    void sample(LocomotionInput input) noexcept;
    [[nodiscard]] LocomotionInput latest() const noexcept { return latest_; }

private:
    LocomotionInput latest_{};
};

struct CharacterMotorConfig final {
    double max_speed_metres_per_second{4.5};
};

class CharacterMotor final {
public:
    explicit CharacterMotor(CharacterMotorConfig config = {});

    void apply_fixed_tick(
        physics::PhysicsWorld& physics_world,
        physics::PhysicsBodyHandle character,
        LocomotionInput input) const;

private:
    CharacterMotorConfig config_;
};

}  // namespace starforge::player
