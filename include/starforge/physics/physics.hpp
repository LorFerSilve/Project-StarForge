#pragma once

#include <compare>
#include <cstddef>
#include <cstdint>
#include <optional>

namespace starforge::physics {

struct Vec3 final {
    double x{0.0};
    double y{0.0};
    double z{0.0};

    friend constexpr auto operator<=>(const Vec3&, const Vec3&) noexcept = default;
};

enum class BodyClass : std::uint8_t {
    Static,
    Kinematic,
    Dynamic,
    Sensor,
    Character,
};

enum class CollisionLayer : std::uint8_t {
    StaticWorld,
    DynamicWorld,
    Character,
    Sensor,
};

struct PhysicsBodyHandle final {
    std::uint64_t scene_generation{0};
    std::uint32_t index{0};
    std::uint32_t generation{0};

    [[nodiscard]] constexpr bool valid() const noexcept {
        return scene_generation != 0U && generation != 0U;
    }
    friend constexpr auto operator<=>(const PhysicsBodyHandle&, const PhysicsBodyHandle&) noexcept = default;
};

struct RaycastHit final {
    PhysicsBodyHandle body{};
    Vec3 position{};
    double fraction{0.0};
};

struct BoxBodyDescriptor final {
    BodyClass body_class{BodyClass::Static};
    CollisionLayer layer{CollisionLayer::StaticWorld};
    Vec3 half_extents{0.5, 0.5, 0.5};
    Vec3 position{};
    Vec3 linear_velocity{};
};

struct CapsuleBodyDescriptor final {
    BodyClass body_class{BodyClass::Character};
    CollisionLayer layer{CollisionLayer::Character};
    double half_height{0.8};
    double radius{0.35};
    Vec3 position{};
    Vec3 linear_velocity{};
};

[[nodiscard]] bool is_valid_descriptor(const BoxBodyDescriptor& descriptor) noexcept;
[[nodiscard]] bool is_valid_descriptor(const CapsuleBodyDescriptor& descriptor) noexcept;

class PhysicsWorld {
public:
    virtual ~PhysicsWorld() = default;

    PhysicsWorld(const PhysicsWorld&) = delete;
    PhysicsWorld& operator=(const PhysicsWorld&) = delete;
    PhysicsWorld(PhysicsWorld&&) = delete;
    PhysicsWorld& operator=(PhysicsWorld&&) = delete;

    [[nodiscard]] virtual std::uint64_t scene_generation() const noexcept = 0;
    [[nodiscard]] virtual PhysicsBodyHandle create_box(const BoxBodyDescriptor& descriptor) = 0;
    [[nodiscard]] virtual PhysicsBodyHandle create_capsule(const CapsuleBodyDescriptor& descriptor) = 0;
    virtual void destroy_body(PhysicsBodyHandle handle) = 0;

    [[nodiscard]] virtual bool contains(PhysicsBodyHandle handle) const noexcept = 0;
    [[nodiscard]] virtual Vec3 position(PhysicsBodyHandle handle) const = 0;
    [[nodiscard]] virtual Vec3 linear_velocity(PhysicsBodyHandle handle) const = 0;
    virtual void set_linear_velocity(PhysicsBodyHandle handle, Vec3 velocity) = 0;
    [[nodiscard]] virtual std::optional<RaycastHit> raycast(Vec3 origin, Vec3 direction) const = 0;

    virtual void step(double seconds) = 0;
    [[nodiscard]] virtual std::size_t body_count() const noexcept = 0;

protected:
    PhysicsWorld() = default;
};

}  // namespace starforge::physics
