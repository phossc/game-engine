#ifndef EDITOR_EDITOR_HPP
#define EDITOR_EDITOR_HPP

#include "engine/core/component.hpp"
#include "engine/core/component_registry.hpp"

namespace editor {

//! Specify all the system components provided by the editor in the
//! DEPENDENCIES() macro.
struct Editor : engine::core::Component {
    COMPONENT("d49d0588-5ed5-46d2-9525-48493a024d93");
    DEPENDENCIES();
};

//! Register all components provided by the editor. Engine and game components
//! can also be overridden here i.e. if you want to provide a custom camera
//! manager.
inline void register_components(engine::core::Component_registry& registry) {
    registry.register_component<Editor>();
}

} // namespace editor

#endif /* EDITOR_EDITOR_HPP */
