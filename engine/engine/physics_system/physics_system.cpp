#include "engine/physics_system/physics_system.hpp"

#include "engine/core/profiler.hpp"
#include "engine/core/system.hpp"
#include "engine/update_priorities.hpp"

namespace engine {

Physics_system::Physics_system()
    : dispatcher_(&collisionConfig_),
      world_(&dispatcher_, &broadphase_, &solver_, &collisionConfig_) {
    world_.setGravity({0.0, -9.8, 0.0});
}

void Physics_system::activate() {
    sys->update_system().register_fixed_update(
            this, static_cast<std::int32_t>(Update_priority::physics));
    sys->update_system().register_interpolation_update(
            this, static_cast<std::int32_t>(Update_priority::physics));
}

void Physics_system::deactivate() {
    sys->update_system().deregister_fixed_update(this);
    sys->update_system().deregister_interpolation_update(this);
}

void Physics_system::fixed_update(double dt) {
    PROFILE("Physics sim", "Fixed update");
    world_.stepSimulation(dt, 1, dt);
}

void Physics_system::interpolation_update(double a) {
    PROFILE("Physics interp", "Interpolation update");
    auto dt = sys->game_loop().simulation_rate();
    double dt_seconds = std::chrono::duration<double>{dt}.count();
    double time_step = dt_seconds * a;
    world_.stepSimulation(time_step, 1, dt_seconds);
}

} // namespace engine