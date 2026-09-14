#include <starforge/physics/physics.hpp>

#include <cmath>

namespace starforge::physics {
namespace {

[[nodiscard]] bool finite(Vec3 value) noexcept {
    return std::isfinite(value.x) && std::isfinite(value.y) && std::isfinite(value.z);
}

}  // namespace

bool is_valid_descriptor(const BoxBodyDescriptor& descriptor) noexcept {
    return finite(descriptor.half_extents) && finite(descriptor.position) && finite(descriptor.linear_velocity) &&
           descriptor.half_extents.x > 0.0 && descriptor.half_extents.y > 0.0 && descriptor.half_extents.z > 0.0;
}

bool is_valid_descriptor(const CapsuleBodyDescriptor& descriptor) noexcept {
    return std::isfinite(descriptor.half_height) && std::isfinite(descriptor.radius) && finite(descriptor.position) &&
           finite(descriptor.linear_velocity) && descriptor.half_height > 0.0 && descriptor.radius > 0.0;
}

}  // namespace starforge::physics
