#ifndef ENGINE_CORE_LAUNCH_HPP
#define ENGINE_CORE_LAUNCH_HPP

#include "engine/core/system.hpp"

#include <exception>
#include <iostream>

namespace engine::core {

template <typename LaunchFirst, typename LaunchSecond>
int launch(int argc, const char** argv) {
    try {
        auto sys_instance = std::make_unique<System>(argc, argv);
        LaunchFirst first;
        LaunchSecond second;

        // Registration order: second -> first -> engine.
        // Allows overriding components in earlier launched systems.
        second.register_components();
        first.register_components();
        sys->register_components();

        // Launch systems.
        sys->attach_system_components();
        first.attach_system_components();
        second.attach_system_components();

        sys->run();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught in launch: " << e.what() << '\n';
        return -1;
    }

    return 0;
}

} // namespace engine::core

#endif /* ENGINE_CORE_LAUNCH_HPP */