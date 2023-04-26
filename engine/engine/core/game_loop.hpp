#ifndef ENGINE_CORE_GAME_LOOP_HPP
#define ENGINE_CORE_GAME_LOOP_HPP

#include <chrono>

namespace engine {

class Game_loop final {
public:
    Game_loop(class Profiler& profiler, class Clock& game_clock, class Ecs& ecs,
              class Update_system& update_system);

    //! Blocks the current thread while running the game loop.
    void run();

    //! Makes run() return in the next iteration.
    void stop() noexcept { running_ = false; }

    //! Specifies the duration between each simulation update.
    void set_simulation_rate(std::chrono::nanoseconds rate) noexcept { simulation_rate_ = rate; }

    std::chrono::nanoseconds simulation_rate() const noexcept { return simulation_rate_; }

private:
    std::chrono::nanoseconds simulation_rate_{16'666'666};
    bool running_ = false;

    class Profiler& profiler_;
    class Clock& game_clock_;
    class Ecs& ecs_;
    class Update_system& update_system_;
};

} // namespace engine

#endif /* ENGINE_CORE_GAME_LOOP_HPP */