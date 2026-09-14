#include <starforge/world/world.hpp>

#include <algorithm>
#include <limits>
#include <stdexcept>

namespace starforge::world {

RuntimeEntityHandle RuntimeEntityRegistry::create() {
    for (std::uint32_t index = 0U; index < slots_.size(); ++index) {
        auto& slot = slots_[index];
        if (slot.alive) {
            continue;
        }
        slot.alive = true;
        ++alive_count_;
        return {.index = index, .generation = slot.generation};
    }

    if (slots_.size() >= static_cast<std::size_t>(std::numeric_limits<std::uint32_t>::max())) {
        throw std::overflow_error{"runtime entity registry exhausted"};
    }

    slots_.push_back(Slot{.generation = 1U, .alive = true});
    ++alive_count_;
    return {.index = static_cast<std::uint32_t>(slots_.size() - 1U), .generation = 1U};
}

void RuntimeEntityRegistry::destroy(RuntimeEntityHandle handle) {
    if (!contains(handle)) {
        throw std::invalid_argument{"stale or invalid runtime entity handle"};
    }

    auto& slot = slots_[handle.index];
    slot.alive = false;
    ++slot.generation;
    if (slot.generation == 0U) {
        slot.generation = 1U;
    }
    --alive_count_;
}

bool RuntimeEntityRegistry::contains(RuntimeEntityHandle handle) const noexcept {
    if (!handle.valid() || handle.index >= slots_.size()) {
        return false;
    }
    const auto& slot = slots_[handle.index];
    return slot.alive && slot.generation == handle.generation;
}

void RuntimeEntityRegistry::clear() noexcept {
    for (auto& slot : slots_) {
        if (!slot.alive) {
            continue;
        }
        slot.alive = false;
        ++slot.generation;
        if (slot.generation == 0U) {
            slot.generation = 1U;
        }
    }
    alive_count_ = 0U;
}

RuntimePosition to_runtime_position(physics::Vec3 context_position, const ContextOrigin& origin) {
    const auto x = context_position.x - origin.x;
    const auto y = context_position.y - origin.y;
    const auto z = context_position.z - origin.z;
    constexpr double max_runtime_coordinate = static_cast<double>(std::numeric_limits<float>::max());
    if (x > max_runtime_coordinate || x < -max_runtime_coordinate || y > max_runtime_coordinate ||
        y < -max_runtime_coordinate || z > max_runtime_coordinate || z < -max_runtime_coordinate) {
        throw std::overflow_error{"context position cannot be represented in runtime coordinates"};
    }
    return {
        .x = static_cast<float>(x),
        .y = static_cast<float>(y),
        .z = static_cast<float>(z),
    };
}

StreamingReadiness::StreamingReadiness(std::span<const RequiredContentKey> required)
    : required_{required.begin(), required.end()} {
    if (std::ranges::any_of(required_, [](RequiredContentKey key) { return key == 0U; })) {
        throw std::invalid_argument{"required content keys must be non-zero"};
    }
}

void StreamingReadiness::mark_resident(RequiredContentKey key) {
    if (key == 0U) {
        throw std::invalid_argument{"resident content key must be non-zero"};
    }
    if (required_.contains(key)) {
        resident_.insert(key);
    }
}

bool StreamingReadiness::ready() const noexcept {
    return missing_count() == 0U;
}

std::size_t StreamingReadiness::missing_count() const noexcept {
    std::size_t missing = 0U;
    for (const auto key : required_) {
        if (!resident_.contains(key)) {
            ++missing;
        }
    }
    return missing;
}

SceneInstance::SceneInstance(
    std::uint64_t scene_generation,
    ContextOrigin origin,
    std::span<const RequiredContentKey> required_content,
    std::unique_ptr<physics::PhysicsWorld> physics_world)
    : scene_generation_{scene_generation}, origin_{origin}, readiness_{required_content}, physics_world_{std::move(physics_world)} {
    if (scene_generation_ == 0U) {
        throw std::invalid_argument{"scene generation must be non-zero"};
    }
    if (origin_.generation == 0U) {
        throw std::invalid_argument{"origin generation must be non-zero"};
    }
    if (!physics_world_) {
        throw std::invalid_argument{"scene requires a physics world"};
    }
    if (physics_world_->scene_generation() != scene_generation_) {
        throw std::invalid_argument{"physics world scene generation mismatch"};
    }
    refresh_readiness();
}

SceneInstance::~SceneInstance() {
    deactivate();
}

void SceneInstance::mark_content_resident(RequiredContentKey key) {
    if (state_ != SceneState::Staging && state_ != SceneState::Ready) {
        throw std::logic_error{"content readiness can only change before activation"};
    }
    readiness_.mark_resident(key);
    refresh_readiness();
}

void SceneInstance::activate() {
    if (state_ != SceneState::Ready) {
        throw std::logic_error{"scene can activate only after all required content is ready"};
    }
    if (!physics_world_) {
        throw std::logic_error{"scene cannot activate without a physics world"};
    }
    state_ = SceneState::Active;
}

void SceneInstance::quiesce() {
    if (state_ != SceneState::Active) {
        throw std::logic_error{"only an active scene can quiesce"};
    }
    state_ = SceneState::Quiescing;
}

void SceneInstance::deactivate() noexcept {
    if (state_ == SceneState::Destroyed) {
        return;
    }
    if (state_ == SceneState::Active) {
        state_ = SceneState::Quiescing;
    }
    entities_.clear();
    physics_world_.reset();
    state_ = SceneState::Destroyed;
}

physics::PhysicsWorld& SceneInstance::physics_world() {
    if (!physics_world_ || (state_ != SceneState::Ready && state_ != SceneState::Active && state_ != SceneState::Quiescing)) {
        throw std::logic_error{"physics world is not available for this scene state"};
    }
    return *physics_world_;
}

const physics::PhysicsWorld& SceneInstance::physics_world() const {
    if (!physics_world_ || (state_ != SceneState::Ready && state_ != SceneState::Active && state_ != SceneState::Quiescing)) {
        throw std::logic_error{"physics world is not available for this scene state"};
    }
    return *physics_world_;
}

void SceneInstance::refresh_readiness() noexcept {
    if (state_ == SceneState::Staging && readiness_.ready()) {
        state_ = SceneState::Ready;
    }
}

}  // namespace starforge::world
