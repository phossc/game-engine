#include "engine/core/system.hpp"

#include "engine/components/transform_component.hpp"
#include "engine/glfw/glfw.hpp"
#include "engine/physics_system/physics_system.hpp"
#include "engine/render_system/camera_manager.hpp"
#include "engine/render_system/render_system.hpp"
#include "engine/window_manager/window.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace engine::core {

System* sys = nullptr;

System::System(int argc, const char** argv)
    : logger_(spdlog::stdout_color_mt("logger")),
      game_loop_(game_clock_, ecs_, update_system_) {
    logger_->flush_on(spdlog::level::warn);

    if (argc <= 0) {
        name_ = "unnamed";
    }
    else {
        name_ = argv[0];
    }

    for (int i = 1; i < argc; ++i) {
        arguments_.emplace_back(argv[i]);
    }

    sys = this;
    entity_ = ecs_.create_entity<System_entity>();
}

System::~System() {
    ecs_.delete_all();
    sys = nullptr;
}

void System::run() {
    entity_->activate();
    game_loop_.run();
}

void System::register_components() {
    component_registry_.register_component<Camera_manager>();
    component_registry_.register_component<Glfw>();
    component_registry_.register_component<Physics_system>();
    component_registry_.register_component<Render_system>();
    component_registry_.register_component<Transform_component>();
    component_registry_.register_component<Window>();
}

void System::attach_system_components() {
    entity_->create<Camera_manager>();
    entity_->create<Glfw>();
    entity_->create<Physics_system>();
    entity_->create<Render_system>();
    entity_->create<Window>();
}

} // namespace engine::core
