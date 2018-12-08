#include <game/game.hpp>

#include <engine/core/system.hpp>

namespace game {

using engine::core::sys;

void Game::register_components() {
    sys->component_registry().register_component<Game>();
}

void Game::attach_system_components() {
    sys->entity().create_component<Game>();
}

void Game::activate() {}

void Game::deactivate() {}

} // namespace game
