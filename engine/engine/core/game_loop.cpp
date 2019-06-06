#include "engine/core/game_loop.hpp"

#include "engine/core/clock.hpp"
#include "engine/core/ecs.hpp"
#include "engine/core/profiler.hpp"
#include "engine/core/update_system.hpp"

namespace engine::core {

Game_loop::Game_loop(Profiler& profiler, Clock& game_clock, Ecs& ecs,
                     Update_system& update_system)
    : profiler_(profiler), game_clock_(game_clock), ecs_(ecs),
      update_system_(update_system) {}

//! For a better understanding of how the game loop works, read this great
//! article https://gafferongames.com/post/fix_your_timestep/ by Glenn Fiedler.
void Game_loop::run() {
    if (running_) {
        return;
    }

    running_ = true;

    std::chrono::nanoseconds accumulated_dt{0};
    auto previous = std::chrono::steady_clock::now();
    while (running_) {
        profiler_.update();
        PROFILE("Frame", {});
        auto current = std::chrono::steady_clock::now();

        //! Slow down the simulation if frame time is too high.
        auto frame_time_max = std::chrono::milliseconds{200};
        auto real_dt = current - previous;
        real_dt = real_dt < frame_time_max ? real_dt : frame_time_max;

        game_clock_.update(real_dt);

        {
            PROFILE("Ecs", "Frame");
            ecs_.update();
        }

        std::chrono::nanoseconds dt = game_clock_.update_dt();
        accumulated_dt += dt;

        double simulation_rate_seconds =
                std::chrono::duration<double>{simulation_rate_}.count();

        while (accumulated_dt >= simulation_rate_) {
            accumulated_dt -= simulation_rate_;

            simulation_rate_seconds =
                    std::chrono::duration<double>{simulation_rate_}.count();

            {
                PROFILE("Fixed update", "Frame");
                update_system_.broadcast_fixed_update(simulation_rate_seconds);
            }
        }

        {
            PROFILE("Interpolation update", "Frame");
            update_system_.broadcast_interpolation_update(
                    std::chrono::duration<double>{accumulated_dt}.count() /
                    simulation_rate_seconds);
        }

        {
            PROFILE("Variable update", "Frame");
            update_system_.broadcast_variable_update(
                    std::chrono::duration<double>{dt}.count());
        }

        previous = current;
    }
}

} // namespace engine::core