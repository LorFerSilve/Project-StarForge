#pragma once

#include <compare>
#include <cstdint>

namespace starforge::core {

inline constexpr std::uint32_t kSimulationHz = 60U;

class SimulationTick {
public:
    using rep_type = std::uint64_t;

    constexpr SimulationTick() noexcept = default;
    explicit constexpr SimulationTick(rep_type value) noexcept : value_(value) {}

    [[nodiscard]] constexpr rep_type raw() const noexcept { return value_; }

    friend constexpr auto operator<=>(SimulationTick, SimulationTick) noexcept = default;

private:
    rep_type value_{0};
};

class SimulationDuration {
public:
    using rep_type = std::int64_t;

    constexpr SimulationDuration() noexcept = default;
    explicit constexpr SimulationDuration(rep_type ticks) noexcept : ticks_(ticks) {}

    [[nodiscard]] static constexpr SimulationDuration from_ticks(rep_type ticks) noexcept {
        return SimulationDuration{ticks};
    }

    [[nodiscard]] constexpr rep_type ticks() const noexcept { return ticks_; }
    [[nodiscard]] constexpr double seconds() const noexcept {
        return static_cast<double>(ticks_) / static_cast<double>(kSimulationHz);
    }

    friend constexpr auto operator<=>(SimulationDuration, SimulationDuration) noexcept = default;

private:
    rep_type ticks_{0};
};

}  // namespace starforge::core
