#ifndef ENGINE_CORE_GAME_LOOP_HPP
#define ENGINE_CORE_GAME_LOOP_HPP

#include <chrono>

namespace engine::core {

class Game_loop final {
public:
    //! Blocks the current thread while running the game loop.
    void run();

    //! Makes run() return in the next iteration.
    void stop() noexcept { running_ = false; }

    //! Specifies the duration between each simulation update.
    void set_simulation_rate(std::chrono::nanoseconds rate) noexcept {
        simulation_rate_ = rate;
    }

    std::chrono::nanoseconds simulation_rate() const noexcept {
        return simulation_rate_;
    }

private:
    std::chrono::nanoseconds simulation_rate_{20'000'000};
    bool running_ = false;
};

} // namespace engine::core

#endif /* ENGINE_CORE_GAME_LOOP_HPP */