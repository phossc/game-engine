#include "engine/render_system/render_system.hpp"

#include "engine/core/system.hpp"

#include <cassert>

namespace engine {

void Render_system::activate() {
    core::sys->update_system().register_variable_update(this, 5000);
}

void Render_system::deactivate() {
    core::sys->update_system().deregister_variable_update(this);
}

void Render_system::variable_update(double dt) {
    for (auto renderer : renderers_) {
        renderer.second->render(dt);
    }
}

void Render_system::register_renderer(Renderer* renderer) {
    assert(renderer != nullptr);
    renderers_.emplace(renderer->priority(), renderer);
}

void Render_system::deregister_renderer(Renderer* renderer) {
    assert(renderer != nullptr);
    renderers_.erase(renderer->priority());
}

} // namespace engine