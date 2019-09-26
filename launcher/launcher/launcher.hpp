#ifndef LAUNCHER_LAUNCHER_HPP
#define LAUNCHER_LAUNCHER_HPP

#include "engine/core/component.hpp"
#include "engine/core/component_registry.hpp"

namespace launcher {

//! Specify all the system components provided by the launcher in the
//! DEPENDENCIES() macro.
struct Launcher : engine::Component {
    COMPONENT("57d6303e-d280-4c6a-8a32-11ee891b80db");
    DEPENDENCIES();
};

//! Register all components provided by the launcher. Engine and game components
//! can also be overridden here i.e. if you want to provide a custom camera
//! manager.
inline void register_components(engine::Component_registry& registry) {
    registry.register_component<Launcher>();
}

} // namespace launcher

#endif /* LAUNCHER_LAUNCHER_HPP */
