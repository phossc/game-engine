#include <launcher/launcher.hpp>

#include <engine/core/system.hpp>

namespace launcher {

using engine::core::sys;
using engine::subsystems::Window_manager;

void Launcher::activate() {
    sys->entity().get<Window_manager>()->create_window(1280, 720);
}

void Launcher::deactivate() {}

} // namespace launcher
