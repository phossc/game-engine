#include "editor/editor_setup.hpp"
#include "engine/core/launch.hpp"
#include "game/game_setup.hpp"

int main(int argc, const char** argv) {
    using editor::Editor_setup;
    using engine::core::launch;
    using game::Game_setup;

    return launch<Editor_setup, Game_setup>(argc, argv);
}