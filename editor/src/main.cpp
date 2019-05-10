#include "editor/editor.hpp"
#include "engine/engine.hpp"
#include "engine/launch.hpp"
#include "game/game.hpp"

int main(int argc, const char** argv) {
    using engine::launch;
    using engine::core::System;
    return launch(argc, argv, [](System& sys_instance) {
        engine::register_components(sys_instance.component_registry());
        game::register_components(sys_instance.component_registry());
        editor::register_components(sys_instance.component_registry());
        sys_instance.entity()->create<engine::Engine>();
        sys_instance.entity()->create<game::Game>();
        sys_instance.entity()->create<editor::Editor>();
    });
}