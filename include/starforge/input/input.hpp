#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <vector>

namespace starforge::input {

enum class ActionId : std::uint8_t {
    MoveForward,
    MoveBackward,
    MoveLeft,
    MoveRight,
    Interact,
    Fire,
    UiConfirm,
    UiCancel,
    Pause,
    Count
};

enum class ContextClass : std::uint8_t {
    SystemModal,
    UI,
    Terminal,
    SpecializedGameplay,
    Restricted,
    OnFoot
};

enum class CaptureMode : std::uint8_t { Exclusive, Conditional, GlobalSystem };
enum class PromptDeviceFamily : std::uint8_t { KeyboardMouse, Controller };

struct ActionState {
    bool pressed{false};
    bool held{false};
    bool released{false};
};

struct ContextBinding {
    ActionId action{ActionId::MoveForward};
    CaptureMode capture{CaptureMode::Exclusive};
    bool handles{true};
};

struct InputContext {
    std::uint64_t id{};
    ContextClass kind{ContextClass::OnFoot};
    std::vector<ContextBinding> bindings{};
};

struct RoutedAction {
    ActionId action{ActionId::MoveForward};
    ActionState state{};
    std::uint64_t owner_context{};
    ContextClass owner_kind{ContextClass::OnFoot};
};

struct RouterDiagnostics {
    std::size_t sampled_actions{};
    std::size_t consumed_actions{};
    std::size_t unhandled_actions{};
    std::uint64_t context_generation{};
};

struct EffectiveBinding {
    ActionId action{ActionId::MoveForward};
    PromptDeviceFamily family{PromptDeviceFamily::KeyboardMouse};
    ContextClass context{ContextClass::OnFoot};
    std::string display{};
};

[[nodiscard]] std::optional<std::string> resolve_prompt(
    ActionId action,
    ContextClass context,
    PromptDeviceFamily family,
    std::span<const EffectiveBinding> bindings);

class SemanticInputRouter final {
  public:
    SemanticInputRouter() = default;

    void set_contexts(std::vector<InputContext> contexts);
    void observe(ActionId action, bool down) noexcept;
    [[nodiscard]] std::vector<RoutedAction> sample_tick();
    void clear_transient_edges() noexcept;

    [[nodiscard]] bool down(ActionId action) const noexcept;
    [[nodiscard]] std::span<const InputContext> contexts() const noexcept { return contexts_; }
    [[nodiscard]] const RouterDiagnostics& diagnostics() const noexcept { return diagnostics_; }

  private:
    struct LatchedAction {
        bool down{false};
        bool pressed{false};
        bool released{false};
    };

    static constexpr std::size_t action_count = static_cast<std::size_t>(ActionId::Count);
    [[nodiscard]] static std::size_t index(ActionId action) noexcept;
    [[nodiscard]] const InputContext* resolve_owner(ActionId action) const noexcept;

    std::array<LatchedAction, action_count> actions_{};
    std::vector<InputContext> contexts_{};
    RouterDiagnostics diagnostics_{};
};

[[nodiscard]] InputContext on_foot_context(std::uint64_t id = 1U);
[[nodiscard]] InputContext focused_ui_context(std::uint64_t id = 2U);

}  // namespace starforge::input
