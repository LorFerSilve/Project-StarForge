#include "starforge/input/input.hpp"

#include <Catch2/catch_test_macros.hpp>

#include <algorithm>

namespace {
[[nodiscard]] const starforge::input::RoutedAction* find_action(
    const std::vector<starforge::input::RoutedAction>& actions,
    const starforge::input::ActionId action) {
    const auto found = std::find_if(actions.begin(), actions.end(), [action](const auto& routed) {
        return routed.action == action;
    });
    return found == actions.end() ? nullptr : &*found;
}
}  // namespace

TEST_CASE("UI context consumes conflicting gameplay actions before on-foot context") {
    starforge::input::SemanticInputRouter router;
    router.set_contexts({starforge::input::on_foot_context(10U),
                         starforge::input::focused_ui_context(20U)});

    router.observe(starforge::input::ActionId::Fire, true);
    const auto actions = router.sample_tick();
    const auto* fire = find_action(actions, starforge::input::ActionId::Fire);
    REQUIRE(fire != nullptr);
    CHECK(fire->owner_context == 20U);
    CHECK(fire->owner_kind == starforge::input::ContextClass::UI);
    CHECK(fire->state.pressed);
    CHECK(fire->state.held);
    CHECK(router.diagnostics().consumed_actions == 1U);
}

TEST_CASE("short press between fixed ticks is exposed exactly once") {
    starforge::input::SemanticInputRouter router;
    router.set_contexts({starforge::input::on_foot_context()});

    router.observe(starforge::input::ActionId::Interact, true);
    router.observe(starforge::input::ActionId::Interact, false);

    const auto first = router.sample_tick();
    const auto* interaction = find_action(first, starforge::input::ActionId::Interact);
    REQUIRE(interaction != nullptr);
    CHECK(interaction->state.pressed);
    CHECK_FALSE(interaction->state.held);
    CHECK(interaction->state.released);

    const auto second = router.sample_tick();
    CHECK(find_action(second, starforge::input::ActionId::Interact) == nullptr);
}

TEST_CASE("held input persists across catch-up ticks without multiplying pressed edge") {
    starforge::input::SemanticInputRouter router;
    router.set_contexts({starforge::input::on_foot_context()});
    router.observe(starforge::input::ActionId::MoveForward, true);

    const auto first = router.sample_tick();
    const auto* first_move = find_action(first, starforge::input::ActionId::MoveForward);
    REQUIRE(first_move != nullptr);
    CHECK(first_move->state.pressed);
    CHECK(first_move->state.held);

    const auto second = router.sample_tick();
    const auto* second_move = find_action(second, starforge::input::ActionId::MoveForward);
    REQUIRE(second_move != nullptr);
    CHECK_FALSE(second_move->state.pressed);
    CHECK(second_move->state.held);
}

TEST_CASE("context transition discards stale destructive press while preserving current hold state") {
    starforge::input::SemanticInputRouter router;
    router.set_contexts({starforge::input::on_foot_context()});
    router.observe(starforge::input::ActionId::Interact, true);

    router.set_contexts({starforge::input::on_foot_context(10U),
                         starforge::input::focused_ui_context(20U)});
    const auto after_transition = router.sample_tick();
    const auto* interaction = find_action(after_transition, starforge::input::ActionId::Interact);
    REQUIRE(interaction != nullptr);
    CHECK_FALSE(interaction->state.pressed);
    CHECK(interaction->state.held);
    CHECK(interaction->owner_context == 20U);

    router.observe(starforge::input::ActionId::Interact, false);
    static_cast<void>(router.sample_tick());
    router.observe(starforge::input::ActionId::Interact, true);
    const auto fresh_press = router.sample_tick();
    interaction = find_action(fresh_press, starforge::input::ActionId::Interact);
    REQUIRE(interaction != nullptr);
    CHECK(interaction->state.pressed);
    CHECK(interaction->owner_context == 20U);
}

TEST_CASE("conditional binding can explicitly pass action to lower-priority context") {
    starforge::input::SemanticInputRouter router;
    starforge::input::InputContext ui{
        .id = 20U,
        .kind = starforge::input::ContextClass::UI,
        .bindings = {{starforge::input::ActionId::Interact,
                      starforge::input::CaptureMode::Conditional,
                      false}},
    };
    router.set_contexts({starforge::input::on_foot_context(10U), ui});
    router.observe(starforge::input::ActionId::Interact, true);

    const auto actions = router.sample_tick();
    const auto* interaction = find_action(actions, starforge::input::ActionId::Interact);
    REQUIRE(interaction != nullptr);
    CHECK(interaction->owner_context == 10U);
    CHECK(interaction->owner_kind == starforge::input::ContextClass::OnFoot);
}
