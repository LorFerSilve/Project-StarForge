#pragma once

#include <cstdint>

namespace starforge::platform {

struct FixedStepPlan final {
    std::uint32_t steps{0};
    double interpolation_alpha{0.0};
    double accepted_frame_seconds{0.0};
};

class FixedStepScheduler final {
public:
    explicit FixedStepScheduler(
        double step_seconds = 1.0 / 60.0,
        double max_frame_seconds = 0.25,
        std::uint32_t max_steps_per_frame = 8);

    [[nodiscard]] FixedStepPlan advance(double frame_seconds) noexcept;
    void reset() noexcept;

    [[nodiscard]] double step_seconds() const noexcept { return step_seconds_; }

private:
    double step_seconds_;
    double max_frame_seconds_;
    std::uint32_t max_steps_per_frame_;
    double accumulator_{0.0};
};

} // namespace starforge::platform
