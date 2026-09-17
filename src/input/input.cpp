#include "starforge/input/input.hpp"

#include <algorithm>
#include <utility>

namespace starforge::input {
namespace {
[[nodiscard]] std::uint8_t context_priority(const ContextClass kind) noexcept {
    return static_cast<std::uint8_t>(kind);
}
}  // namespace

std::optional<std::string> resolve_prompt(const ActionId action,
                                          const ContextClass context,
                                          const PromptDeviceFamily family,
                                          const std::span<const EffectiveBinding> bindings) {
    const auto binding = std::find_if(bindings.begin(), bindings.end(), [=](const EffectiveBinding& candidate) {
        return candidate.action == action && candidate.context == context && candidate.family == family &&
               !candidate.display.empty();
    });
    if (binding == bindings.end()) {
        return std::nullopt;
    }
    return binding->display;
}

std::size_t SemanticInputRouter::index(const ActionId action) noexcept {
    return static_cast<std::size_t>(action);
}

void SemanticInputRouter::set_contexts(std::vector<InputContext> contexts) {
    std::stable_sort(contexts.begin(), contexts.end(), [](const InputContext& lhs, const InputContext& rhs) {
        return context_priority(lhs.kind) < context_priority(rhs.kind);
    });
    contexts_ = std::move(contexts);
    clear_transient_edges();
    ++diagnostics_.context_generation;
}

void SemanticInputRouter::observe(const ActionId action, const bool down_now) noexcept {
    const auto action_index = index(action);
    if (action_index >= actions_.size()) {
        return;
    }
    auto& state = actions_[action_index];
    if (down_now != state.down) {
        if (down_now) {
            state.pressed = true;
        } else {
            state.released = true;
        }
        state.down = down_now;
    }
}

const InputContext* SemanticInputRouter::resolve_owner(const ActionId action) const noexcept {
    for (const auto& context : contexts_) {
        const auto binding = std::find_if(context.bindings.begin(), context.bindings.end(), [action](const ContextBinding& candidate) {
            return candidate.action == action;
        });
        if (binding == context.bindings.end()) {
            continue;
        }
        if (binding->capture == CaptureMode::Conditional && !binding->handles) {
            continue;
        }
        return &context;
    }
    return nullptr;
}

std::vector<RoutedAction> SemanticInputRouter::sample_tick() {
    std::vector<RoutedAction> routed;
    routed.reserve(actions_.size());
    diagnostics_.sampled_actions = 0U;
    diagnostics_.consumed_actions = 0U;
    diagnostics_.unhandled_actions = 0U;

    for (std::size_t action_index = 0; action_index < actions_.size(); ++action_index) {
        auto& latched = actions_[action_index];
        if (!latched.down && !latched.pressed && !latched.released) {
            continue;
        }
        ++diagnostics_.sampled_actions;
        const auto action = static_cast<ActionId>(action_index);
        const auto* owner = resolve_owner(action);
        if (owner == nullptr) {
            ++diagnostics_.unhandled_actions;
        } else {
            routed.push_back({
                .action = action,
                .state = {.pressed = latched.pressed, .held = latched.down, .released = latched.released},
                .owner_context = owner->id,
                .owner_kind = owner->kind,
            });
            ++diagnostics_.consumed_actions;
        }
        latched.pressed = false;
        latched.released = false;
    }
    return routed;
}

void SemanticInputRouter::clear_transient_edges() noexcept {
    for (auto& action : actions_) {
        action.pressed = false;
        action.released = false;
    }
}

bool SemanticInputRouter::down(const ActionId action) const noexcept {
    const auto action_index = index(action);
    return action_index < actions_.size() && actions_[action_index].down;
}

InputContext on_foot_context(const std::uint64_t id) {
    return {
        .id = id,
        .kind = ContextClass::OnFoot,
        .bindings = {
            {ActionId::MoveForward, CaptureMode::Exclusive, true},
            {ActionId::MoveBackward, CaptureMode::Exclusive, true},
            {ActionId::MoveLeft, CaptureMode::Exclusive, true},
            {ActionId::MoveRight, CaptureMode::Exclusive, true},
            {ActionId::Interact, CaptureMode::Exclusive, true},
            {ActionId::Fire, CaptureMode::Exclusive, true},
            {ActionId::Pause, CaptureMode::GlobalSystem, true},
        },
    };
}

InputContext focused_ui_context(const std::uint64_t id) {
    return {
        .id = id,
        .kind = ContextClass::UI,
        .bindings = {
            {ActionId::Interact, CaptureMode::Exclusive, true},
            {ActionId::Fire, CaptureMode::Exclusive, true},
            {ActionId::UiConfirm, CaptureMode::Exclusive, true},
            {ActionId::UiCancel, CaptureMode::Exclusive, true},
        },
    };
}

}  // namespace starforge::input
