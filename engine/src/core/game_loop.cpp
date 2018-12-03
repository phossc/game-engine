#include <engine/core/game_loop.hpp>

#include <engine/core/system.hpp>

namespace engine::core {

void Game_loop::run() {
    if (running_) {
        return;
    }

    running_ = true;

    std::chrono::nanoseconds accumulated_dt{0};
    auto previous = std::chrono::steady_clock::now();
    while (running_) {
        auto current = std::chrono::steady_clock::now();

        sys->game_clock().update(current - previous);
        sys->ecs().update();

        std::chrono::nanoseconds dt = sys->game_clock().update_dt();
        accumulated_dt += dt;

        double simulation_rate_seconds =
                std::chrono::duration<double>{simulation_rate_}.count();

        while (accumulated_dt >= simulation_rate_) {
            accumulated_dt -= simulation_rate_;

            simulation_rate_seconds =
                    std::chrono::duration<double>{simulation_rate_}.count();

            sys->update_system().broadcast_fixed_update(
                    simulation_rate_seconds);
        }

        sys->update_system().broadcast_interpolation_update(
                std::chrono::duration<double>{accumulated_dt}.count() /
                simulation_rate_seconds);

        sys->update_system().broadcast_variable_update(
                std::chrono::duration<double>{dt}.count());

        previous = current;
    }
}

} // namespace engine::core