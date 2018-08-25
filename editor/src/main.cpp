#include <exception>
#include <iostream>

#include <editor/editor.hpp>
#include <engine/core/system.hpp>
#include <game/game.hpp>

int main(int argc, const char** argv) {
    try {
        engine::core::sys = std::make_unique<engine::core::System>(argc, argv);
        game::Game game;
        editor::Editor editor;

        // Registration order: game -> editor -> engine.
        // Allows the game and editor to override engine components.
        game.register_components();
        editor.register_components();
        engine::core::sys->register_components();

        engine::core::sys->attach_system_components();
        editor.attach_system_components();
        game.attach_system_components();

        engine::core::sys->run();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught in main: " << e.what() << '\n';
    }

    engine::core::sys = nullptr;
}
