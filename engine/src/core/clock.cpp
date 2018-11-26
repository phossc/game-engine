#include <engine/core/clock.hpp>

#include <cstdint>

namespace engine::core {

void Clock::update(std::chrono::nanoseconds realtime_dt) noexcept {
    if (paused_) {
        return;
    }

    // Due to the small values of realtime_dt there should be no significant
    // loss of information. Only fractions of nanoseconds will be truncated
    // which is acceptable.
    std::chrono::nanoseconds::rep scaled_dt = realtime_dt.count() * timescale_;

    total_time_ += std::chrono::nanoseconds{scaled_dt};
}

void Clock::step_once() noexcept {
    if (!paused_) {
        return;
    }

    std::chrono::nanoseconds::rep scaled_step =
            step_duration_.count() * timescale_;

    total_time_ += std::chrono::nanoseconds{scaled_step};
}

} // namespace engine::core