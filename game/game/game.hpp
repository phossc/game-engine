#ifndef GAME_GAME_HPP
#define GAME_GAME_HPP

#include "engine/core/component.hpp"
#include "engine/core/component_registry.hpp"

namespace game {

//! Specify all the system components provided by the game in the
//! DEPENDENCIES() macro.
struct Game : engine::Component {
    COMPONENT("b1aec0b5-06ad-4ee5-8578-3e0b30cbb1ca");
    DEPENDENCIES();
};

//! Register all components provided by the game. Engine components can also be
//! overridden here i.e. if you want to provide a custom camera manager.
inline void register_components(engine::Component_registry& registry) {
    registry.register_component<Game>();
}

} // namespace game

#endif /* GAME_GAME_HPP */
