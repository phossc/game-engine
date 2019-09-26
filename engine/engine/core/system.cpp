#include "engine/core/system.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace engine {

System* sys = nullptr;

System::System(int argc, const char** argv)
    : logger_(spdlog::stdout_color_mt("logger")),
      game_loop_(profiler_, game_clock_, ecs_, update_system_) {
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

} // namespace engine
