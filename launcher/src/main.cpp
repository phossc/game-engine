#include <exception>
#include <iostream>

#include <engine/core/system.hpp>
#include <game/game.hpp>
#include <launcher/launcher.hpp>

int main(int argc, const char** argv) {
    try {
        auto sys_instance = std::make_unique<engine::core::System>(argc, argv);
        game::Game game;
        launcher::Launcher launcher;

        // Registration order: game -> launcher -> engine.
        // Allows the game and launcher to override engine components.
        game.register_components();
        launcher.register_components();
        engine::core::sys->register_components();

        engine::core::sys->attach_system_components();
        launcher.attach_system_components();
        game.attach_system_components();

        engine::core::sys->run();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught in main: " << e.what() << '\n';
    }
}
