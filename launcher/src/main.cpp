#include "engine/core/launch.hpp"
#include "game/game_setup.hpp"
#include "launcher/launcher_setup.hpp"

int main(int argc, const char** argv) {
    using engine::core::launch;
    using game::Game_setup;
    using launcher::Launcher_setup;

    return launch<Launcher_setup, Game_setup>(argc, argv);
}