#ifndef ENGINE_CORE_CLOCK_HPP
#define ENGINE_CORE_CLOCK_HPP

#include <chrono>

namespace engine::core {

class Clock final {
public:
    void update(std::chrono::nanoseconds realtime_dt) noexcept;
    void step_once() noexcept;

    void pause() noexcept { paused_ = true; };
    void unpause() noexcept { paused_ = false; };
    bool paused() const noexcept { return paused_; }

    std::chrono::nanoseconds total_time() const noexcept { return total_time_; }

    void set_timescale(double scale) noexcept { timescale_ = scale; }
    double timescale() const noexcept { return timescale_; };

    void set_step_duration(std::chrono::nanoseconds duration) noexcept {
        step_duration_ = duration;
    };

    std::chrono::nanoseconds step_duration() const noexcept {
        return step_duration_;
    }

private:
    bool paused_ = false;
    double timescale_ = 1.0;
    std::chrono::nanoseconds total_time_{0};
    std::chrono::nanoseconds step_duration_{20'000'000};
};

} // namespace engine::core

#endif /* ENGINE_CORE_CLOCK_HPP */