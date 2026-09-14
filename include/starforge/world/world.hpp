#pragma once

#include <starforge/physics/physics.hpp>

#include <compare>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <span>
#include <unordered_set>
#include <vector>

namespace starforge::world {

enum class SceneState : std::uint8_t {
    Staging,
    Ready,
    Activating,
    Active,
    Quiescing,
    Deactivating,
    Destroyed,
};

struct RuntimeEntityHandle final {
    std::uint32_t index{0};
    std::uint32_t generation{0};
    [[nodiscard]] constexpr bool valid() const noexcept { return generation != 0U; }
    friend constexpr auto operator<=>(const RuntimeEntityHandle&, const RuntimeEntityHandle&) noexcept = default;
};

class RuntimeEntityRegistry final {
public:
    [[nodiscard]] RuntimeEntityHandle create();
    void destroy(RuntimeEntityHandle handle);
    [[nodiscard]] bool contains(RuntimeEntityHandle handle) const noexcept;
    [[nodiscard]] std::size_t size() const noexcept { return alive_count_; }
    void clear() noexcept;

private:
    struct Slot final { std::uint32_t generation{1U}; bool alive{false}; };
    std::vector<Slot> slots_;
    std::size_t alive_count_{0U};
};

struct ContextOrigin final {
    double x{0.0};
    double y{0.0};
    double z{0.0};
    std::uint64_t generation{1U};
};

struct RuntimePosition final { float x{0.0F}; float y{0.0F}; float z{0.0F}; };
[[nodiscard]] RuntimePosition to_runtime_position(physics::Vec3 context_position, const ContextOrigin& origin);

using RequiredContentKey = std::uint64_t;

class StreamingReadiness final {
public:
    explicit StreamingReadiness(std::span<const RequiredContentKey> required);
    void mark_resident(RequiredContentKey key);
    [[nodiscard]] bool ready() const noexcept;
    [[nodiscard]] bool hard_hold() const noexcept { return !ready(); }
    [[nodiscard]] std::size_t missing_count() const noexcept;

private:
    std::unordered_set<RequiredContentKey> required_;
    std::unordered_set<RequiredContentKey> resident_;
};

class SceneInstance final {
public:
    SceneInstance(std::uint64_t scene_generation, ContextOrigin origin,
                  std::span<const RequiredContentKey> required_content,
                  std::unique_ptr<physics::PhysicsWorld> physics_world);
    ~SceneInstance();

    SceneInstance(const SceneInstance&) = delete;
    SceneInstance& operator=(const SceneInstance&) = delete;
    SceneInstance(SceneInstance&&) = delete;
    SceneInstance& operator=(SceneInstance&&) = delete;

    void mark_content_resident(RequiredContentKey key);
    void activate();
    void quiesce();
    void deactivate() noexcept;

    [[nodiscard]] SceneState state() const noexcept { return state_; }
    [[nodiscard]] bool hard_streaming_hold() const noexcept { return readiness_.hard_hold(); }
    [[nodiscard]] std::uint64_t scene_generation() const noexcept { return scene_generation_; }
    [[nodiscard]] const ContextOrigin& origin() const noexcept { return origin_; }
    [[nodiscard]] RuntimeEntityRegistry& entities() noexcept { return entities_; }
    [[nodiscard]] const RuntimeEntityRegistry& entities() const noexcept { return entities_; }
    [[nodiscard]] physics::PhysicsWorld& physics_world();
    [[nodiscard]] const physics::PhysicsWorld& physics_world() const;

private:
    void refresh_readiness() noexcept;
    std::uint64_t scene_generation_;
    ContextOrigin origin_;
    StreamingReadiness readiness_;
    std::unique_ptr<physics::PhysicsWorld> physics_world_;
    RuntimeEntityRegistry entities_;
    SceneState state_{SceneState::Staging};
};

enum class ActiveLocalContextKind : std::uint8_t {
    Horizon,
    Mission,
    PersistentLocation,
    Recovery,
};

struct ActiveLocalContextDescriptor final {
    ActiveLocalContextKind kind{ActiveLocalContextKind::Horizon};
    std::uint64_t owner_key{0};
    std::uint64_t scene_generation{0};
    std::uint64_t origin_generation{0};
    std::uint64_t scene_profile_key{0};
};

class ActiveLocalContext final {
public:
    void activate(ActiveLocalContextDescriptor descriptor, std::unique_ptr<SceneInstance> scene);
    void deactivate() noexcept;

    [[nodiscard]] bool has_active_scene() const noexcept;
    [[nodiscard]] const std::optional<ActiveLocalContextDescriptor>& descriptor() const noexcept { return descriptor_; }
    [[nodiscard]] SceneInstance& scene();
    [[nodiscard]] const SceneInstance& scene() const;

private:
    std::optional<ActiveLocalContextDescriptor> descriptor_;
    std::unique_ptr<SceneInstance> scene_;
};

}  // namespace starforge::world
