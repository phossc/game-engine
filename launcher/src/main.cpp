#include <engine/core/launch.hpp>
#include <game/game.hpp>
#include <launcher/launcher.hpp>

int main(int argc, const char** argv) {
    using engine::core::launch;
    using game::Game;
    using launcher::Launcher;

    return launch<Launcher, Game>(argc, argv);
}