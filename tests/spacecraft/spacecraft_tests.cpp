#include "starforge/spacecraft/spacecraft.hpp"

#include <catch2/catch_test_macros.hpp>

using namespace starforge::spacecraft;

namespace {
ShipConfiguration config() {
    return ShipConfiguration{1000.0, 500.0, 10000.0, 5000.0, 4.0, 2.0, 100.0, 1.5, 25.0};
}
}

TEST_CASE("ship mass affects acceleration and cargo ownership is unique", "[spacecraft]") {
    SpacecraftRuntime light{ShipId{1}, config()};
    SpacecraftRuntime loaded{ShipId{2}, config()};
    REQUIRE(loaded.load_cargo(CargoRecord{CargoId{10}, 500.0}));
    REQUIRE_FALSE(loaded.load_cargo(CargoRecord{CargoId{10}, 1.0}));
    FlightInput input{};
    input.translation.z = 1.0;
    light.simulate(input, 1.0);
    loaded.simulate(input, 1.0);
    REQUIRE(light.flight().linear_velocity.z > loaded.flight().linear_velocity.z);
    REQUIRE(light.flight().linear_velocity.z == 10.0);
}

TEST_CASE("flight assist uses maneuver authority while assist-off preserves inertia", "[spacecraft]") {
    SpacecraftRuntime ship{ShipId{1}, config()};
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

TEST_CASE("avionics enforces local speed envelope without instant stopping", "[spacecraft]") {
    SpacecraftRuntime ship{ShipId{1}, config()};
    FlightInput thrust{};
    thrust.translation.z = 1.0;
    for (int i = 0; i < 100; ++i) ship.simulate(thrust, 0.25);
    REQUIRE(ship.flight().linear_velocity.z <= 100.0);
    const double before = ship.flight().linear_velocity.z;
    ship.simulate({}, 0.1);
    REQUIRE(ship.flight().linear_velocity.z > 0.0);
    REQUIRE(ship.flight().linear_velocity.z < before);
}

TEST_CASE("docking requires bounded envelope and freezes local motion", "[spacecraft]") {
    SpacecraftRuntime ship{ShipId{1}, config()};
    REQUIRE_FALSE(ship.dock(DockId{2}, 3.0, 0.2));
    REQUIRE_FALSE(ship.dock(DockId{2}, 1.0, 2.0));
    REQUIRE(ship.dock(DockId{2}, 1.0, 0.5));
    REQUIRE(ship.docked_at() == DockId{2});
    const auto position = ship.flight().position;
    FlightInput input{};
    input.translation.z = 1.0;
    ship.simulate(input, 1.0);
    REQUIRE(ship.flight().position == position);
    REQUIRE(ship.undock());
}

TEST_CASE("cargo capacity and transfer preserve ownership", "[spacecraft]") {
    SpacecraftRuntime ship{ShipId{1}, config()};
    REQUIRE(ship.load_cargo(CargoRecord{CargoId{7}, 400.0}));
    REQUIRE_FALSE(ship.load_cargo(CargoRecord{CargoId{8}, 101.0}));
    auto cargo = ship.unload_cargo(CargoId{7});
    REQUIRE(cargo);
    REQUIRE(cargo.value().id == CargoId{7});
    REQUIRE_FALSE(ship.unload_cargo(CargoId{7}));
}

TEST_CASE("strategic reach gates travel", "[spacecraft]") {
    SpacecraftRuntime ship{ShipId{1}, config()};
    REQUIRE(ship.strategic_travel(25.0));
    REQUIRE_FALSE(ship.strategic_travel(25.1));
}

TEST_CASE("spacecraft state survives serialization without duplicating cargo", "[spacecraft]") {
    SpacecraftRuntime ship{ShipId{42}, config()};
    REQUIRE(ship.load_cargo(CargoRecord{CargoId{9}, 125.0}));
    FlightInput input{};
    input.translation.x = 0.5;
    input.rotation.y = 0.25;
    ship.simulate(input, 0.5);
    REQUIRE(ship.dock(DockId{3}, 0.5, 0.25));
    const auto encoded = ship.serialize();
    auto restored = SpacecraftRuntime::deserialize(encoded);
    REQUIRE(restored);
    REQUIRE(restored.value().id() == ShipId{42});
    REQUIRE(restored.value().current_mass_kg() == 1125.0);
    REQUIRE(restored.value().docked_at() == DockId{3});
    REQUIRE(restored.value().flight().position == ship.flight().position);
    auto cargo = restored.value().unload_cargo(CargoId{9});
    REQUIRE(cargo);
    REQUIRE_FALSE(restored.value().unload_cargo(CargoId{9}));
}

TEST_CASE("invalid serialized state fails closed", "[spacecraft]") {
    REQUIRE_FALSE(SpacecraftRuntime::deserialize("not-a-ship"));
}
