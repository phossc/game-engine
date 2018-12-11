#include <editor/editor.hpp>
#include <engine/core/launch.hpp>
#include <game/game.hpp>

int main(int argc, const char** argv) {
    using editor::Editor;
    using engine::core::launch;
    using game::Game;

    return launch<Editor, Game>(argc, argv);
}