#include <starforge/platform/fixed_step.hpp>

#include <algorithm>
#include <cmath>

namespace starforge::platform {

FixedStepScheduler::FixedStepScheduler(
    const double step_seconds,
    const double max_frame_seconds,
    const std::uint32_t max_steps_per_frame)
    : step_seconds_(step_seconds > 0.0 ? step_seconds : 1.0 / 60.0),
      max_frame_seconds_(max_frame_seconds > 0.0 ? max_frame_seconds : 0.25),
      max_steps_per_frame_(std::max<std::uint32_t>(1, max_steps_per_frame)) {}

FixedStepPlan FixedStepScheduler::advance(const double frame_seconds) noexcept {
    const auto finite_frame = std::isfinite(frame_seconds) ? frame_seconds : 0.0;
    const auto accepted = std::clamp(finite_frame, 0.0, max_frame_seconds_);
    accumulator_ += accepted;

    std::uint32_t steps = 0;
    while (accumulator_ >= step_seconds_ && steps < max_steps_per_frame_) {
        accumulator_ -= step_seconds_;
        ++steps;
    }

    // A per-frame step cap limits how much authoritative simulation work may be
    // performed in one rendered frame; it must not discard queued simulation
    // time. Only the fractional remainder participates in render interpolation
    // while any complete queued ticks remain in the accumulator for later
    // frames.
    const auto interpolation_remainder = std::fmod(accumulator_, step_seconds_);

    return FixedStepPlan{
        .steps = steps,
        .interpolation_alpha = interpolation_remainder / step_seconds_,
        .accepted_frame_seconds = accepted,
    };
}

void FixedStepScheduler::reset() noexcept {
    accumulator_ = 0.0;
}

} // namespace starforge::platform
