#include <starforge/physics/jolt_world.hpp>

#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/PhysicsSystem.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <utility>
#include <vector>

namespace starforge::physics {
namespace {

namespace object_layers {
constexpr JPH::ObjectLayer static_world = 0;
constexpr JPH::ObjectLayer dynamic_world = 1;
constexpr JPH::ObjectLayer character = 2;
constexpr JPH::ObjectLayer sensor = 3;
constexpr JPH::ObjectLayer count = 4;
}  // namespace object_layers

namespace broadphase_layers {
const JPH::BroadPhaseLayer non_moving{0};
const JPH::BroadPhaseLayer moving{1};
constexpr JPH::uint count = 2;
}  // namespace broadphase_layers

class BroadPhaseLayerInterface final : public JPH::BroadPhaseLayerInterface {
public:
    BroadPhaseLayerInterface() {
        mapping_[object_layers::static_world] = broadphase_layers::non_moving;
        mapping_[object_layers::dynamic_world] = broadphase_layers::moving;
        mapping_[object_layers::character] = broadphase_layers::moving;
        mapping_[object_layers::sensor] = broadphase_layers::moving;
    }

    [[nodiscard]] JPH::uint GetNumBroadPhaseLayers() const override { return broadphase_layers::count; }

    [[nodiscard]] JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer layer) const override {
        if (layer >= object_layers::count) {
            return broadphase_layers::moving;
        }
        return mapping_[layer];
    }

private:
    std::array<JPH::BroadPhaseLayer, object_layers::count> mapping_{};
};

class ObjectVsBroadPhaseLayerFilter final : public JPH::ObjectVsBroadPhaseLayerFilter {
public:
    [[nodiscard]] bool ShouldCollide(JPH::ObjectLayer object_layer, JPH::BroadPhaseLayer broadphase_layer) const override {
        if (object_layer == object_layers::static_world) {
            return broadphase_layer == broadphase_layers::moving;
        }
        return true;
    }
};

class ObjectLayerPairFilter final : public JPH::ObjectLayerPairFilter {
public:
    [[nodiscard]] bool ShouldCollide(JPH::ObjectLayer lhs, JPH::ObjectLayer rhs) const override {
        if (lhs == object_layers::static_world && rhs == object_layers::static_world) {
            return false;
        }
        if (lhs == object_layers::sensor && rhs == object_layers::sensor) {
            return false;
        }
        return true;
    }
};

struct JoltRuntimeState final {
    std::mutex mutex;
    std::size_t users{0};
};

JoltRuntimeState& runtime_state() {
    static JoltRuntimeState state;
    return state;
}

void acquire_jolt_runtime() {
    auto& state = runtime_state();
    const std::scoped_lock lock{state.mutex};
    if (state.users == 0U) {
        JPH::RegisterDefaultAllocator();
        JPH::Factory::sInstance = new JPH::Factory();
        JPH::RegisterTypes();
    }
    ++state.users;
}

void release_jolt_runtime() noexcept {
    auto& state = runtime_state();
    const std::scoped_lock lock{state.mutex};
    if (state.users == 0U) {
        return;
    }
    --state.users;
    if (state.users == 0U) {
        JPH::UnregisterTypes();
        delete JPH::Factory::sInstance;
        JPH::Factory::sInstance = nullptr;
    }
}

[[nodiscard]] JPH::ObjectLayer to_jolt_layer(CollisionLayer layer) {
    switch (layer) {
    case CollisionLayer::StaticWorld:
        return object_layers::static_world;
    case CollisionLayer::DynamicWorld:
        return object_layers::dynamic_world;
    case CollisionLayer::Character:
        return object_layers::character;
    case CollisionLayer::Sensor:
        return object_layers::sensor;
    }
    throw std::invalid_argument{"unsupported collision layer"};
}

[[nodiscard]] JPH::EMotionType to_motion_type(BodyClass body_class) {
    switch (body_class) {
    case BodyClass::Static:
        return JPH::EMotionType::Static;
    case BodyClass::Kinematic:
    case BodyClass::Sensor:
        return JPH::EMotionType::Kinematic;
    case BodyClass::Dynamic:
    case BodyClass::Character:
        return JPH::EMotionType::Dynamic;
    }
    throw std::invalid_argument{"unsupported body class"};
}

[[nodiscard]] JPH::RVec3 to_jolt_position(Vec3 value) {
    return {static_cast<JPH::Real>(value.x), static_cast<JPH::Real>(value.y), static_cast<JPH::Real>(value.z)};
}

[[nodiscard]] JPH::Vec3 to_jolt_vector(Vec3 value) {
    return {static_cast<float>(value.x), static_cast<float>(value.y), static_cast<float>(value.z)};
}

[[nodiscard]] Vec3 from_jolt_position(JPH::RVec3Arg value) {
    return {static_cast<double>(value.GetX()), static_cast<double>(value.GetY()), static_cast<double>(value.GetZ())};
}

[[nodiscard]] Vec3 from_jolt_vector(JPH::Vec3Arg value) {
    return {static_cast<double>(value.GetX()), static_cast<double>(value.GetY()), static_cast<double>(value.GetZ())};
}

class JoltPhysicsWorld final : public PhysicsWorld {
public:
    explicit JoltPhysicsWorld(std::uint64_t scene_generation)
        : scene_generation_{scene_generation}, temp_allocator_{4U * 1024U * 1024U},
          job_system_{JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, 1} {
        if (scene_generation_ == 0U) {
            throw std::invalid_argument{"physics world requires a non-zero scene generation"};
        }

        acquire_jolt_runtime();
        try {
            physics_system_.Init(
                4096,
                0,
                4096,
                2048,
                broadphase_layer_interface_,
                object_vs_broadphase_layer_filter_,
                object_layer_pair_filter_);
            physics_system_.SetGravity({0.0F, -9.81F, 0.0F});
        } catch (...) {
            release_jolt_runtime();
            throw;
        }
    }

    ~JoltPhysicsWorld() override {
        auto& body_interface = physics_system_.GetBodyInterface();
        for (auto& slot : slots_) {
            if (!slot.alive) {
                continue;
            }
            body_interface.RemoveBody(slot.body_id);
            body_interface.DestroyBody(slot.body_id);
            slot.alive = false;
        }
        alive_count_ = 0U;
        release_jolt_runtime();
    }

    [[nodiscard]] std::uint64_t scene_generation() const noexcept override { return scene_generation_; }

    [[nodiscard]] PhysicsBodyHandle create_box(const BoxBodyDescriptor& descriptor) override {
        if (!is_valid_descriptor(descriptor)) {
            throw std::invalid_argument{"invalid box body descriptor"};
        }

        const auto half_extents = JPH::Vec3{
            static_cast<float>(descriptor.half_extents.x),
            static_cast<float>(descriptor.half_extents.y),
            static_cast<float>(descriptor.half_extents.z),
        };
        const auto* shape = new JPH::BoxShape{half_extents};
        return create_body(shape, descriptor.body_class, descriptor.layer, descriptor.position, descriptor.linear_velocity);
    }

    [[nodiscard]] PhysicsBodyHandle create_capsule(const CapsuleBodyDescriptor& descriptor) override {
        if (!is_valid_descriptor(descriptor)) {
            throw std::invalid_argument{"invalid capsule body descriptor"};
        }

        const auto* shape = new JPH::CapsuleShape{
            static_cast<float>(descriptor.half_height),
            static_cast<float>(descriptor.radius),
        };
        return create_body(shape, descriptor.body_class, descriptor.layer, descriptor.position, descriptor.linear_velocity);
    }

    void destroy_body(PhysicsBodyHandle handle) override {
        auto& slot = require_slot(handle);
        auto& body_interface = physics_system_.GetBodyInterface();
        body_interface.RemoveBody(slot.body_id);
        body_interface.DestroyBody(slot.body_id);
        slot.alive = false;
        ++slot.generation;
        if (slot.generation == 0U) {
            slot.generation = 1U;
        }
        --alive_count_;
    }

    [[nodiscard]] bool contains(PhysicsBodyHandle handle) const noexcept override {
        if (!handle.valid() || handle.index >= slots_.size()) {
            return false;
        }
        const auto& slot = slots_[handle.index];
        return slot.alive && slot.generation == handle.generation;
    }

    [[nodiscard]] Vec3 position(PhysicsBodyHandle handle) const override {
        const auto& slot = require_slot(handle);
        return from_jolt_position(physics_system_.GetBodyInterface().GetPosition(slot.body_id));
    }

    [[nodiscard]] Vec3 linear_velocity(PhysicsBodyHandle handle) const override {
        const auto& slot = require_slot(handle);
        return from_jolt_vector(physics_system_.GetBodyInterface().GetLinearVelocity(slot.body_id));
    }

    void set_linear_velocity(PhysicsBodyHandle handle, Vec3 velocity) override {
        const auto& slot = require_slot(handle);
        physics_system_.GetBodyInterface().SetLinearVelocity(slot.body_id, to_jolt_vector(velocity));
    }

    void step(double seconds) override {
        if (!(seconds > 0.0) || seconds > 0.25) {
            throw std::invalid_argument{"physics step duration must be in (0, 0.25] seconds"};
        }
        physics_system_.Update(static_cast<float>(seconds), 1, &temp_allocator_, &job_system_);
    }

    [[nodiscard]] std::size_t body_count() const noexcept override { return alive_count_; }

private:
    struct BodySlot final {
        JPH::BodyID body_id;
        std::uint32_t generation{1U};
        bool alive{true};
    };

    [[nodiscard]] PhysicsBodyHandle create_body(
        const JPH::Shape* shape,
        BodyClass body_class,
        CollisionLayer layer,
        Vec3 position,
        Vec3 linear_velocity) {
        JPH::BodyCreationSettings settings{
            shape,
            to_jolt_position(position),
            JPH::Quat::sIdentity(),
            to_motion_type(body_class),
            to_jolt_layer(layer),
        };
        settings.mIsSensor = body_class == BodyClass::Sensor;
        if (body_class == BodyClass::Character) {
            settings.mAllowedDOFs = JPH::EAllowedDOFs::TranslationX | JPH::EAllowedDOFs::TranslationY |
                                    JPH::EAllowedDOFs::TranslationZ;
            settings.mFriction = 0.0F;
        }

        auto& body_interface = physics_system_.GetBodyInterface();
        const auto body_id = body_interface.CreateAndAddBody(settings, JPH::EActivation::Activate);
        if (body_id.IsInvalid()) {
            throw std::runtime_error{"Jolt failed to allocate a physics body"};
        }
        body_interface.SetLinearVelocity(body_id, to_jolt_vector(linear_velocity));

        const auto index = static_cast<std::uint32_t>(slots_.size());
        slots_.push_back(BodySlot{.body_id = body_id});
        ++alive_count_;
        return {.index = index, .generation = slots_.back().generation};
    }

    [[nodiscard]] BodySlot& require_slot(PhysicsBodyHandle handle) {
        if (!contains(handle)) {
            throw std::invalid_argument{"stale or invalid physics body handle"};
        }
        return slots_[handle.index];
    }

    [[nodiscard]] const BodySlot& require_slot(PhysicsBodyHandle handle) const {
        if (!contains(handle)) {
            throw std::invalid_argument{"stale or invalid physics body handle"};
        }
        return slots_[handle.index];
    }

    std::uint64_t scene_generation_;
    BroadPhaseLayerInterface broadphase_layer_interface_;
    ObjectVsBroadPhaseLayerFilter object_vs_broadphase_layer_filter_;
    ObjectLayerPairFilter object_layer_pair_filter_;
    JPH::TempAllocatorImpl temp_allocator_;
    JPH::JobSystemThreadPool job_system_;
    JPH::PhysicsSystem physics_system_;
    std::vector<BodySlot> slots_;
    std::size_t alive_count_{0U};
};

}  // namespace

std::unique_ptr<PhysicsWorld> create_jolt_physics_world(std::uint64_t scene_generation) {
    return std::make_unique<JoltPhysicsWorld>(scene_generation);
}

}  // namespace starforge::physics
