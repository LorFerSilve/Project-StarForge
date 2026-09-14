#include <starforge/player/character_motor.hpp>

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace starforge::player {

void InputSampleBuffer::sample(LocomotionInput input) noexcept {
    latest_.move_x = std::clamp(input.move_x, -1.0F, 1.0F);
    latest_.move_z = std::clamp(input.move_z, -1.0F, 1.0F);
}

CharacterMotor::CharacterMotor(CharacterMotorConfig config) : config_{config} {
    if (!std::isfinite(config_.max_speed_metres_per_second) || config_.max_speed_metres_per_second <= 0.0) {
        throw std::invalid_argument{"character motor max speed must be finite and positive"};
    }
}

void CharacterMotor::apply_fixed_tick(
    physics::PhysicsWorld& physics_world,
    physics::PhysicsBodyHandle character,
    LocomotionInput input) const {
    if (!physics_world.contains(character)) {
        throw std::invalid_argument{"character motor received a stale physics handle"};
    }

    double x = std::clamp(static_cast<double>(input.move_x), -1.0, 1.0);
    double z = std::clamp(static_cast<double>(input.move_z), -1.0, 1.0);
    const double length_squared = x * x + z * z;
    if (length_squared > 1.0) {
        const double inverse_length = 1.0 / std::sqrt(length_squared);
        x *= inverse_length;
        z *= inverse_length;
    }

    auto velocity = physics_world.linear_velocity(character);
    velocity.x = x * config_.max_speed_metres_per_second;
    velocity.z = z * config_.max_speed_metres_per_second;
    physics_world.set_linear_velocity(character, velocity);
}

}  // namespace starforge::player
