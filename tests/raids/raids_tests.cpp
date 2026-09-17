#include "starforge/raids/raids.hpp"

#include <catch2/catch_test_macros.hpp>

using namespace starforge::raids;

TEST_CASE("defense resolution is deterministic for equal state and steps", "[raids][determinism]") {
    DefenseEventRecord event{DefenseEventId{1}, 42};
    RaidRuntime first;
    RaidRuntime second;
    REQUIRE(first.add_defense_event(event));
    REQUIRE(second.add_defense_event(event));
    const DefenseStep step{3, 7, 4, true};
    REQUIRE(first.advance_defense(event.id, step, 10));
    REQUIRE(second.advance_defense(event.id, step, 10));
    CHECK(first.serialize() == second.serialize());
}

TEST_CASE("stolen cargo is lost exactly once only after hostile extraction", "[raids][ownership]") {
    DefenseEventRecord event{DefenseEventId{2}, 7};
    event.cargo.push_back({CargoId{11}, CargoOwner::Station, 0});
    RaidRuntime runtime;
    REQUIRE(runtime.add_defense_event(event));
    REQUIRE(runtime.steal_cargo(event.id, CargoId{11}, 100));
    CHECK(runtime.defense_event(event.id)->cargo.front().owner == CargoOwner::AttackerCarrier);
    REQUIRE(runtime.commit_hostile_extraction(event.id, CargoId{11}, 101));
    CHECK(runtime.defense_event(event.id)->cargo.front().owner == CargoOwner::Extracted);
    CHECK_FALSE(runtime.commit_hostile_extraction(event.id, CargoId{11}, 101));
}

TEST_CASE("reinforcement arrivals are finite", "[raids][reinforcements]") {
    DefenseEventRecord event{DefenseEventId{3}, 9};
    RaidRuntime runtime;
    REQUIRE(runtime.add_defense_event(event));
    REQUIRE(runtime.call_reinforcements(event.id, 2));
    for (int i = 0; i < 6; ++i) {
        const auto result = runtime.advance_reinforcements(event.id);
        if (!result) break;
    }
    const auto* current = runtime.defense_event(event.id);
    REQUIRE(current != nullptr);
    CHECK(current->reinforcements_arrived == 2);
    CHECK(current->reinforcement == ReinforcementState::Arrived);
    CHECK_FALSE(runtime.advance_reinforcements(event.id));
}

TEST_CASE("offscreen defense state round trips without reroll", "[raids][persistence]") {
    DefenseEventRecord event{DefenseEventId{4}, 1234};
    event.cargo.push_back({CargoId{22}, CargoOwner::Station, 0});
    RaidRuntime runtime;
    REQUIRE(runtime.add_defense_event(event));
    REQUIRE(runtime.advance_defense(event.id, DefenseStep{1, 5, 8, true}, 20));
    REQUIRE(runtime.steal_cargo(event.id, CargoId{22}, 200));
    const auto snapshot = runtime.serialize();
    const auto restored = RaidRuntime::deserialize(snapshot);
    REQUIRE(restored);
    CHECK(restored.value().serialize() == snapshot);
}

TEST_CASE("failed defense establishes recovery grace", "[raids][recovery]") {
    DefenseEventRecord event{DefenseEventId{5}, 55};
    RaidRuntime runtime;
    REQUIRE(runtime.add_defense_event(event));
    REQUIRE(runtime.resolve_defense(event.id, Resolution::Failed, 100));
    CHECK_FALSE(runtime.can_generate_severe_station_attack(101));
    CHECK(runtime.can_generate_severe_station_attack(100 + 60ULL * 60ULL * 10ULL));
}

TEST_CASE("boarding requires a physical route", "[raids][defense]") {
    DefenseEventRecord event{DefenseEventId{6}, 77};
    event.phase = DefensePhase::ExternalDefense;
    RaidRuntime runtime;
    REQUIRE(runtime.add_defense_event(event));
    REQUIRE(runtime.advance_defense(event.id, DefenseStep{1, 5, 0, false}, 1));
    CHECK(runtime.defense_event(event.id)->phase == DefensePhase::Withdrawal);
}
