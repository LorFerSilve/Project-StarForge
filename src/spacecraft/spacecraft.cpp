#include "starforge/spacecraft/spacecraft.hpp"

#include <algorithm>
#include <cmath>
#include <sstream>

namespace starforge::spacecraft {
namespace {

double clamp_unit(double value) noexcept { return std::clamp(value, -1.0, 1.0); }
double magnitude(const Vec3& value) noexcept {
    return std::sqrt(value.x * value.x + value.y * value.y + value.z * value.z);
}
void clamp_speed(Vec3& velocity, double limit) noexcept {
    const double speed = magnitude(velocity);
    if (limit > 0.0 && speed > limit) {
        const double scale = limit / speed;
        velocity.x *= scale;
        velocity.y *= scale;
        velocity.z *= scale;
    }
}
void approach_zero(double& value, double amount) noexcept {
    if (value > 0.0) value = std::max(0.0, value - amount);
    else if (value < 0.0) value = std::min(0.0, value + amount);
}

}  // namespace

SpacecraftRuntime::SpacecraftRuntime(ShipId id, ShipConfiguration configuration)
    : id_(id), configuration_(configuration) {}

double SpacecraftRuntime::cargo_mass_kg() const noexcept {
    double mass = 0.0;
    for (const auto& item : cargo_) mass += item.mass_kg;
    return mass;
}

double SpacecraftRuntime::current_mass_kg() const noexcept {
    return configuration_.dry_mass_kg + cargo_mass_kg();
}

void SpacecraftRuntime::set_flight_assist(bool enabled) noexcept {
    if (flight_.flight_assist != enabled) {
        flight_.flight_assist = enabled;
        ++revision_;
    }
}

void SpacecraftRuntime::set_systems(ShipSystems systems) noexcept {
    systems_.power = std::clamp(systems.power, 0.0, 1.0);
    systems_.propulsion = std::clamp(systems.propulsion, 0.0, 1.0);
    systems_.maneuvering = std::clamp(systems.maneuvering, 0.0, 1.0);
    systems_.avionics = std::clamp(systems.avionics, 0.0, 1.0);
    ++revision_;
}

void SpacecraftRuntime::simulate(const FlightInput& input, double seconds) noexcept {
    if (seconds <= 0.0 || docked_at_) return;
    const double mass = std::max(1.0, current_mass_kg());
    const double power = systems_.power;
    const double maneuver = systems_.maneuvering * power;
    const double propulsion = systems_.propulsion * power;
    const double boost = input.boost ? std::max(1.0, configuration_.boost_multiplier) : 1.0;
    const double forward_accel = configuration_.forward_thrust_n * propulsion * boost / mass;
    const double lateral_accel = configuration_.maneuver_thrust_n * maneuver / mass;

    flight_.linear_velocity.x += clamp_unit(input.translation.x) * lateral_accel * seconds;
    flight_.linear_velocity.y += clamp_unit(input.translation.y) * lateral_accel * seconds;
    flight_.linear_velocity.z += clamp_unit(input.translation.z) * forward_accel * seconds;

    const bool no_translation = input.translation.x == 0.0 && input.translation.y == 0.0 &&
                                input.translation.z == 0.0;
    if ((flight_.flight_assist && no_translation) || input.brake) {
        const double braking = lateral_accel * seconds;
        approach_zero(flight_.linear_velocity.x, braking);
        approach_zero(flight_.linear_velocity.y, braking);
        approach_zero(flight_.linear_velocity.z, braking);
    }

    const double angular_accel = configuration_.rotational_thrust * maneuver /
                                 std::max(1.0, configuration_.inertia);
    flight_.angular_velocity.x += clamp_unit(input.rotation.x) * angular_accel * seconds;
    flight_.angular_velocity.y += clamp_unit(input.rotation.y) * angular_accel * seconds;
    flight_.angular_velocity.z += clamp_unit(input.rotation.z) * angular_accel * seconds;
    const bool no_rotation = input.rotation.x == 0.0 && input.rotation.y == 0.0 && input.rotation.z == 0.0;
    if (flight_.flight_assist && no_rotation) {
        const double stabilization = angular_accel * seconds;
        approach_zero(flight_.angular_velocity.x, stabilization);
        approach_zero(flight_.angular_velocity.y, stabilization);
        approach_zero(flight_.angular_velocity.z, stabilization);
    }

    const double speed_limit = configuration_.cruise_speed_mps * systems_.avionics *
                               (input.boost ? std::max(1.0, configuration_.boost_multiplier) : 1.0);
    clamp_speed(flight_.linear_velocity, speed_limit);
    flight_.position.x += flight_.linear_velocity.x * seconds;
    flight_.position.y += flight_.linear_velocity.y * seconds;
    flight_.position.z += flight_.linear_velocity.z * seconds;
    flight_.orientation.x += flight_.angular_velocity.x * seconds;
    flight_.orientation.y += flight_.angular_velocity.y * seconds;
    flight_.orientation.z += flight_.angular_velocity.z * seconds;
    ++revision_;
}

core::Result<void, ShipError> SpacecraftRuntime::load_cargo(CargoRecord cargo) {
    if (!cargo.id.valid() || cargo.mass_kg < 0.0) return core::Result<void, ShipError>::failure(ShipError::InvalidId);
    if (std::ranges::any_of(cargo_, [&](const auto& item) { return item.id == cargo.id; }))
        return core::Result<void, ShipError>::failure(ShipError::CargoAlreadyOwned);
    if (cargo_mass_kg() + cargo.mass_kg > configuration_.cargo_capacity_kg)
        return core::Result<void, ShipError>::failure(ShipError::CargoCapacityExceeded);
    cargo_.push_back(cargo);
    ++revision_;
    return core::Result<void, ShipError>::success();
}

core::Result<CargoRecord, ShipError> SpacecraftRuntime::unload_cargo(CargoId cargo_id) {
    const auto it = std::ranges::find_if(cargo_, [&](const auto& item) { return item.id == cargo_id; });
    if (it == cargo_.end()) return core::Result<CargoRecord, ShipError>::failure(ShipError::CargoNotOwned);
    const CargoRecord cargo = *it;
    cargo_.erase(it);
    ++revision_;
    return core::Result<CargoRecord, ShipError>::success(cargo);
}

core::Result<void, ShipError> SpacecraftRuntime::dock(DockId dock_id, double distance_m,
                                                       double relative_speed_mps) {
    if (!dock_id.valid()) return core::Result<void, ShipError>::failure(ShipError::InvalidId);
    if (docked_at_) return core::Result<void, ShipError>::failure(ShipError::AlreadyDocked);
    if (distance_m < 0.0 || distance_m > 2.0 || relative_speed_mps < 0.0 || relative_speed_mps > 1.0)
        return core::Result<void, ShipError>::failure(ShipError::InvalidDockingEnvelope);
    docked_at_ = dock_id;
    flight_.linear_velocity = {};
    flight_.angular_velocity = {};
    ++revision_;
    return core::Result<void, ShipError>::success();
}

core::Result<void, ShipError> SpacecraftRuntime::undock() {
    if (!docked_at_) return core::Result<void, ShipError>::failure(ShipError::NotDocked);
    docked_at_.reset();
    ++revision_;
    return core::Result<void, ShipError>::success();
}

core::Result<void, ShipError> SpacecraftRuntime::strategic_travel(double distance) {
    if (distance < 0.0 || distance > configuration_.reach)
        return core::Result<void, ShipError>::failure(ShipError::InsufficientReach);
    ++revision_;
    return core::Result<void, ShipError>::success();
}

std::string SpacecraftRuntime::serialize() const {
    std::ostringstream out;
    out.precision(17);
    out << "SFSHIP1 " << id_.raw() << ' ' << revision_ << ' ' << configuration_.dry_mass_kg << ' '
        << configuration_.cargo_capacity_kg << ' ' << configuration_.forward_thrust_n << ' '
        << configuration_.maneuver_thrust_n << ' ' << configuration_.rotational_thrust << ' '
        << configuration_.inertia << ' ' << configuration_.cruise_speed_mps << ' '
        << configuration_.boost_multiplier << ' ' << configuration_.reach << ' '
        << systems_.power << ' ' << systems_.propulsion << ' ' << systems_.maneuvering << ' '
        << systems_.avionics << ' ' << flight_.position.x << ' ' << flight_.position.y << ' '
        << flight_.position.z << ' ' << flight_.linear_velocity.x << ' ' << flight_.linear_velocity.y << ' '
        << flight_.linear_velocity.z << ' ' << flight_.orientation.x << ' ' << flight_.orientation.y << ' '
        << flight_.orientation.z << ' ' << flight_.angular_velocity.x << ' ' << flight_.angular_velocity.y << ' '
        << flight_.angular_velocity.z << ' ' << flight_.flight_assist << ' ' << docked_at_.value_or(DockId{}).raw()
        << ' ' << cargo_.size();
    for (const auto& cargo : cargo_) out << ' ' << cargo.id.raw() << ' ' << cargo.mass_kg;
    return out.str();
}

core::Result<SpacecraftRuntime, ShipError> SpacecraftRuntime::deserialize(std::string_view data) {
    std::istringstream in{std::string(data)};
    std::string magic;
    std::uint64_t id_raw{}, revision{}, dock_raw{};
    ShipConfiguration config;
    ShipSystems systems;
    FlightState flight;
    std::size_t cargo_count{};
    if (!(in >> magic >> id_raw >> revision >> config.dry_mass_kg >> config.cargo_capacity_kg >>
          config.forward_thrust_n >> config.maneuver_thrust_n >> config.rotational_thrust >> config.inertia >>
          config.cruise_speed_mps >> config.boost_multiplier >> config.reach >> systems.power >> systems.propulsion >>
          systems.maneuvering >> systems.avionics >> flight.position.x >> flight.position.y >> flight.position.z >>
          flight.linear_velocity.x >> flight.linear_velocity.y >> flight.linear_velocity.z >> flight.orientation.x >>
          flight.orientation.y >> flight.orientation.z >> flight.angular_velocity.x >> flight.angular_velocity.y >>
          flight.angular_velocity.z >> flight.flight_assist >> dock_raw >> cargo_count) || magic != "SFSHIP1" || id_raw == 0)
        return core::Result<SpacecraftRuntime, ShipError>::failure(ShipError::InvalidSerializedState);
    SpacecraftRuntime runtime{ShipId{id_raw}, config};
    runtime.revision_ = revision;
    runtime.systems_ = systems;
    runtime.flight_ = flight;
    if (dock_raw != 0) runtime.docked_at_ = DockId{dock_raw};
    for (std::size_t index = 0; index < cargo_count; ++index) {
        std::uint64_t cargo_id{};
        double mass{};
        if (!(in >> cargo_id >> mass) || cargo_id == 0 || mass < 0.0)
            return core::Result<SpacecraftRuntime, ShipError>::failure(ShipError::InvalidSerializedState);
        runtime.cargo_.push_back(CargoRecord{CargoId{cargo_id}, mass});
    }
    if (runtime.current_mass_kg() <= 0.0 || runtime.cargo_mass_kg() > config.cargo_capacity_kg)
        return core::Result<SpacecraftRuntime, ShipError>::failure(ShipError::InvalidSerializedState);
    return core::Result<SpacecraftRuntime, ShipError>::success(std::move(runtime));
}

}  // namespace starforge::spacecraft
