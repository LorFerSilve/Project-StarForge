#pragma once

#include "starforge/core/result.hpp"
#include "starforge/core/strong_id.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace starforge::spacecraft {

struct ShipTag;
struct DockTag;
struct CargoTag;
using ShipId = core::PersistentId<ShipTag>;
using DockId = core::PersistentId<DockTag>;
using CargoId = core::PersistentId<CargoTag>;

struct Vec3 final {
    double x{};
    double y{};
    double z{};
    friend bool operator==(const Vec3&, const Vec3&) = default;
};

struct FlightInput final {
    Vec3 translation{};
    Vec3 rotation{};
    bool brake{false};
    bool boost{false};
};

struct ShipConfiguration final {
    double dry_mass_kg{1000.0};
    double cargo_capacity_kg{0.0};
    double forward_thrust_n{0.0};
    double maneuver_thrust_n{0.0};
    double rotational_thrust{0.0};
    double inertia{1.0};
    double cruise_speed_mps{0.0};
    double boost_multiplier{1.0};
    double reach{0.0};
};

struct ShipSystems final {
    double power{1.0};
    double propulsion{1.0};
    double maneuvering{1.0};
    double avionics{1.0};
};

struct CargoRecord final {
    CargoId id{};
    double mass_kg{};
};

struct FlightState final {
    Vec3 position{};
    Vec3 linear_velocity{};
    Vec3 orientation{};
    Vec3 angular_velocity{};
    bool flight_assist{true};
};

enum class ShipError : std::uint8_t {
    InvalidConfiguration,
    InvalidId,
    CargoAlreadyOwned,
    CargoNotOwned,
    CargoCapacityExceeded,
    InvalidDockingEnvelope,
    AlreadyDocked,
    NotDocked,
    InsufficientReach,
    InvalidSerializedState,
};

class SpacecraftRuntime final {
public:
    explicit SpacecraftRuntime(ShipId id, ShipConfiguration configuration);

    [[nodiscard]] ShipId id() const noexcept { return id_; }
    [[nodiscard]] const ShipConfiguration& configuration() const noexcept { return configuration_; }
    [[nodiscard]] const ShipSystems& systems() const noexcept { return systems_; }
    [[nodiscard]] const FlightState& flight() const noexcept { return flight_; }
    [[nodiscard]] double current_mass_kg() const noexcept;
    [[nodiscard]] double cargo_mass_kg() const noexcept;
    [[nodiscard]] std::optional<DockId> docked_at() const noexcept { return docked_at_; }
    [[nodiscard]] std::uint64_t revision() const noexcept { return revision_; }

    void set_flight_assist(bool enabled) noexcept;
    void set_systems(ShipSystems systems) noexcept;
    void simulate(const FlightInput& input, double seconds) noexcept;

    [[nodiscard]] core::Result<void, ShipError> load_cargo(CargoRecord cargo);
    [[nodiscard]] core::Result<CargoRecord, ShipError> unload_cargo(CargoId cargo_id);
    [[nodiscard]] core::Result<void, ShipError> dock(DockId dock_id, double distance_m,
                                                     double relative_speed_mps);
    [[nodiscard]] core::Result<void, ShipError> undock();
    [[nodiscard]] core::Result<void, ShipError> strategic_travel(double distance);

    [[nodiscard]] std::string serialize() const;
    [[nodiscard]] static core::Result<SpacecraftRuntime, ShipError> deserialize(std::string_view data);

private:
    ShipId id_{};
    ShipConfiguration configuration_{};
    ShipSystems systems_{};
    FlightState flight_{};
    std::vector<CargoRecord> cargo_{};
    std::optional<DockId> docked_at_{};
    std::uint64_t revision_{1};
};

}  // namespace starforge::spacecraft
