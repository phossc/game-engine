#ifndef ENGINE_ENGINE_HPP
#define ENGINE_ENGINE_HPP

#include "engine/components/transform_c.hpp"
#include "engine/core/component.hpp"
#include "engine/core/component_registry.hpp"
#include "engine/glfw/glfw.hpp"
#include "engine/input_system/input_system.hpp"
#include "engine/physics_system/physics_system.hpp"
#include "engine/render_system/camera_manager.hpp"
#include "engine/render_system/render_system.hpp"
#include "engine/window_manager/window.hpp"

namespace engine {

//! Specify all the system components provided by the engine in the
//! DEPENDENCIES() macro.
struct Engine : core::Component {
    COMPONENT("887587e4-b56b-4359-9e0f-d81b0b652ed6");
    DEPENDENCIES(Camera_manager::uuid_s(), Glfw::uuid_s(),
                 Input_system::uuid_s(), Physics_system::uuid_s(),
                 Render_system::uuid_s(), Window::uuid_s());
};

//! Register all components provided by the engine.
inline void register_components(core::Component_registry& registry) {
    registry.register_component<Camera_manager>();
    registry.register_component<Engine>();
    registry.register_component<Glfw>();
    registry.register_component<Input_system>();
    registry.register_component<Physics_system>();
    registry.register_component<Render_system>();
    registry.register_component<Transform_c>();
    registry.register_component<Window>();
}

} // namespace engine

#endif /* ENGINE_ENGINE_HPP */