#include <editor/editor_setup.hpp>

#include <editor/editor.hpp>
#include <engine/core/system.hpp>

namespace editor {

using engine::core::sys;

void Editor_setup::register_components() {
    sys->component_registry().register_component<Editor>();
}

void Editor_setup::attach_system_components() {
    sys->entity().create<Editor>();
}

} // namespace editor