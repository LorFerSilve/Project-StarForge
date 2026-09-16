#include "starforge/spacecraft/spacecraft.hpp"

#include <catch2/catch_test_macros.hpp>

#include <cmath>

using namespace starforge::spacecraft;

namespace {
ShipConfiguration config() {
    return ShipConfiguration{1000.0, 500.0, 10000.0, 5000.0, 4.0, 2.0, 100.0, 1.5, ReachClass::II};
}
DockingCapture valid_capture(DockId id = DockId{2}) {
    return DockingCapture{id, 1.0, 0.5, 2.0, 0.05, true, true, true};
}
}

TEST_CASE("ship mass affects acceleration and cargo ownership is globally unique", "[spacecraft]") {
    CargoOwnershipStore ownership;
    SpacecraftRuntime light{ShipId{1}, config(), ownership};
    SpacecraftRuntime loaded{ShipId{2}, config(), ownership};
    REQUIRE(loaded.load_cargo(CargoRecord{CargoId{10}, 500.0}));
    REQUIRE_FALSE(light.load_cargo(CargoRecord{CargoId{10}, 1.0}));
    FlightInput input{};
    input.translation.z = 1.0;
    light.simulate(input, 1.0);
    loaded.simulate(input, 1.0);
    REQUIRE(light.flight().linear_velocity.z > loaded.flight().linear_velocity.z);
    REQUIRE(light.flight().linear_velocity.z == 10.0);
}

TEST_CASE("translation follows the rotated ship frame", "[spacecraft]") {
    CargoOwnershipStore ownership;
    SpacecraftRuntime ship{ShipId{1}, config(), ownership};
    FlightInput yaw{};
    yaw.rotation.y = 1.0;
    ship.simulate(yaw, 0.5);
    ship.set_flight_assist(false);
    FlightInput forward{};
    forward.translation.z = 1.0;
    ship.simulate(forward, 0.1);
    REQUIRE(ship.flight().linear_velocity.x > 0.4);
    REQUIRE(ship.flight().linear_velocity.z > 0.8);
}

TEST_CASE("flight assist uses maneuver authority while assist-off preserves inertia", "[spacecraft]") {
    CargoOwnershipStore ownership;
    SpacecraftRuntime ship{ShipId{1}, config(), ownership};
    FlightInput thrust{};
    thrust.translation.z = 1.0;
    ship.simulate(thrust, 1.0);
    const double moving = ship.flight().linear_velocity.z;
    ship.set_flight_assist(false);
    ship.simulate({}, 1.0);
    REQUIRE(ship.flight().linear_velocity.z == moving);
    ship.set_flight_assist(true);
    ship.simulate({}, 1.0);
    REQUIRE(ship.flight().linear_velocity.z < moving);
    ShipSystems damaged{};
    damaged.maneuvering = 0.0;
    ship.set_systems(damaged);
    const double before = ship.flight().linear_velocity.z;
    ship.simulate({}, 1.0);
    REQUIRE(ship.flight().linear_velocity.z == before);
}

TEST_CASE("speed envelope recovery is bounded by maneuver authority", "[spacecraft]") {
    CargoOwnershipStore ownership;
    SpacecraftRuntime ship{ShipId{1}, config(), ownership};
    FlightInput boost{};
    boost.translation.z = 1.0;
    boost.boost = true;
    for (int i = 0; i < 100; ++i) ship.simulate(boost, 0.25);
    const double boosted = ship.flight().linear_velocity.z;
    REQUIRE(boosted > 100.0);
    ShipSystems no_maneuver{};
    no_maneuver.maneuvering = 0.0;
    ship.set_systems(no_maneuver);
    ship.simulate({}, 0.1);
    REQUIRE(ship.flight().linear_velocity.z == boosted);
}

TEST_CASE("docking validates the complete capture envelope and freezes local motion", "[spacecraft]") {
    CargoOwnershipStore ownership;
    SpacecraftRuntime ship{ShipId{1}, config(), ownership};
    auto invalid = valid_capture();
    invalid.orientation_error_degrees = 6.0;
    REQUIRE_FALSE(ship.dock(invalid));
    invalid = valid_capture();
    invalid.connector_compatible = false;
    REQUIRE_FALSE(ship.dock(invalid));
    invalid = valid_capture();
    invalid.unobstructed = false;
    REQUIRE_FALSE(ship.dock(invalid));
    invalid = valid_capture();
    invalid.operable = false;
    REQUIRE_FALSE(ship.dock(invalid));
    REQUIRE(ship.dock(valid_capture()));
    REQUIRE(ship.docked_at() == DockId{2});
    const auto position = ship.flight().position;
    FlightInput input{};
    input.translation.z = 1.0;
    ship.simulate(input, 1.0);
    REQUIRE(ship.flight().position == position);
    REQUIRE(ship.undock());
}

TEST_CASE("cargo capacity and transfer preserve authoritative ownership", "[spacecraft]") {
    CargoOwnershipStore ownership;
    SpacecraftRuntime source{ShipId{1}, config(), ownership};
    SpacecraftRuntime destination{ShipId{2}, config(), ownership};
    REQUIRE(source.load_cargo(CargoRecord{CargoId{7}, 400.0}));
    REQUIRE_FALSE(destination.load_cargo(CargoRecord{CargoId{7}, 400.0}));
    REQUIRE_FALSE(source.load_cargo(CargoRecord{CargoId{8}, 101.0}));
    auto cargo = source.unload_cargo(CargoId{7});
    REQUIRE(cargo);
    REQUIRE(destination.load_cargo(cargo.value()));
    REQUIRE(ownership.owner(CargoId{7}) == ShipId{2});
}

TEST_CASE("strategic reach uses route capability class and availability", "[spacecraft]") {
    CargoOwnershipStore ownership;
    SpacecraftRuntime ship{ShipId{1}, config(), ownership};
    REQUIRE(ship.strategic_travel(RouteRequirement{ReachClass::I, true}));
    REQUIRE(ship.strategic_travel(RouteRequirement{ReachClass::II, true}));
    REQUIRE_FALSE(ship.strategic_travel(RouteRequirement{ReachClass::III, true}));
    REQUIRE_FALSE(ship.strategic_travel(RouteRequirement{ReachClass::I, false}));
}

TEST_CASE("spacecraft state survives serialization without duplicating cargo", "[spacecraft]") {
    CargoOwnershipStore ownership;
    SpacecraftRuntime ship{ShipId{42}, config(), ownership};
    REQUIRE(ship.load_cargo(CargoRecord{CargoId{9}, 125.0}));
    FlightInput input{};
    input.translation.x = 0.5;
    input.rotation.y = 0.25;
    ship.simulate(input, 0.5);
    REQUIRE(ship.dock(valid_capture(DockId{3})));
    const auto encoded = ship.serialize();
    REQUIRE(ship.unload_cargo(CargoId{9}));
    auto restored = SpacecraftRuntime::deserialize(encoded, ownership);
    REQUIRE(restored);
    REQUIRE(restored.value().id() == ShipId{42});
    REQUIRE(restored.value().current_mass_kg() == 1125.0);
    REQUIRE(restored.value().docked_at() == DockId{3});
    REQUIRE(restored.value().flight().position == ship.flight().position);
    auto cargo = restored.value().unload_cargo(CargoId{9});
    REQUIRE(cargo);
    REQUIRE_FALSE(restored.value().unload_cargo(CargoId{9}));
}

TEST_CASE("invalid serialized state and duplicate cargo IDs fail closed", "[spacecraft]") {
    CargoOwnershipStore ownership;
    REQUIRE_FALSE(SpacecraftRuntime::deserialize("not-a-ship", ownership));
    const std::string duplicate =
        "SFSHIP2 42 1 1000 500 10000 5000 4 2 100 1.5 2 1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 1 0 2 9 100 9 100";
    REQUIRE_FALSE(SpacecraftRuntime::deserialize(duplicate, ownership));
}
