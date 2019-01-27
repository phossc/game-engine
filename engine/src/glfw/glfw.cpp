#include "engine/glfw/glfw.hpp"

#include "engine/core/system.hpp"

#include "GLFW/glfw3.h"

#include <stdexcept>

namespace engine {

static void error_callback(int, const char* description) {
    throw std::runtime_error(description);
}

void Glfw::activate() {
    glfwSetErrorCallback(error_callback);
    glfwInit();
    core::sys->update_system().register_variable_update(this, 0);
}

void Glfw::deactivate() {
    core::sys->update_system().deregister_variable_update(this);
    glfwTerminate();
}

void Glfw::variable_update(double) {
    glfwPollEvents();
}

} // namespace engine
