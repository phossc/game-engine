#include "engine/render_system/render_system.hpp"

#include "engine/core/system.hpp"
#include "engine/update_priorities.hpp"

#include "glad/glad.h"

#include <cassert>

namespace engine {

void Render_system::activate() {
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    core::sys->update_system().register_variable_update(
            this, static_cast<std::int32_t>(Update_priority::rendering));
}

void Render_system::deactivate() {
    core::sys->update_system().deregister_variable_update(this);
}

void Render_system::variable_update(double dt) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (auto renderer : renderers_) {
        renderer.second->render(dt);
    }
    dependency<Window>()->swap_buffers();
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