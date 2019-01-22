#include "engine/core/clock.hpp"

#include <cstdint>

namespace engine::core {

void Clock::update(std::chrono::nanoseconds dt) noexcept {
    if (paused_) {
        refresh_update_dt();
        return;
    }

    // Due to the small values of realtime_dt there should be no significant
    // loss of information. Only fractions of nanoseconds will be truncated
    // which is acceptable.
    std::chrono::nanoseconds::rep scaled_dt = dt.count() * timescale_;

    total_time_ += std::chrono::nanoseconds{scaled_dt};
    refresh_update_dt();
}

void Clock::step_once() noexcept {
    if (!paused_) {
        return;
    }

    std::chrono::nanoseconds::rep scaled_step =
            step_duration_.count() * timescale_;

    total_time_ += std::chrono::nanoseconds{scaled_step};
}

void Clock::refresh_update_dt() noexcept {
    update_dt_ = total_time_ - previous_total_time_;
    previous_total_time_ = total_time_;
}

} // namespace engine::core