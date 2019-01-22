#include "game/game_setup.hpp"

#include "engine/core/system.hpp"
#include "game/game.hpp"

namespace game {

using engine::core::sys;

void Game_setup::register_components() {
    sys->component_registry().register_component<Game>();
}

void Game_setup::attach_system_components() {
    sys->entity()->create<Game>();
}

} // namespace game