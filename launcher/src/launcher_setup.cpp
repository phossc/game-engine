#include "launcher/launcher_setup.hpp"

#include "engine/core/system.hpp"
#include "launcher/launcher.hpp"

namespace launcher {

using engine::core::sys;

void Launcher_setup::register_components() {
    sys->component_registry().register_component<Launcher>();
}

void Launcher_setup::attach_system_components() {
    sys->entity()->create<Launcher>();
}

} // namespace launcher
