#include <engine/core/system.hpp>

#include <engine/core/system_entity.hpp>

namespace engine::core {

std::unique_ptr<System> sys = nullptr;

System::System(int argc, const char** argv) {
    if (argc <= 0) {
        name_ = "unnamed";
    }
    else {
        name_ = argv[0];
    }

    for (int i = 1; i < argc; ++i) {
        arguments_.emplace_back(argv[i]);
    }

    entity_ = ecs_.create_entity<System_entity>();
}

void System::run() {
    entity_->activate();
    game_loop_.run();
}

void System::register_components() {}
void System::attach_system_components() {}

} // namespace engine::core
