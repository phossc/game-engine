#ifndef ENGINE_LAUNCH_HPP
#define ENGINE_LAUNCH_HPP

#include "engine/core/system.hpp"

#include <exception>
#include <functional>
#include <iostream>

namespace engine {

int launch(int argc, const char** argv, std::function<void(System&)> setup) {
    try {
        System sys_instance(argc, argv);
        setup(sys_instance);
        sys_instance.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught in launch: " << e.what() << '\n';
        return -1;
    }

    return 0;
}

} // namespace engine

#endif /* ENGINE_LAUNCH_HPP */