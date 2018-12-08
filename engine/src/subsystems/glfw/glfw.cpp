#include <engine/subsystems/glfw.hpp>

#include <stdexcept>

#include <GLFW/glfw3.h>
#include <engine/core/system.hpp>

namespace engine::subsystems {

static void error_callback(int error, const char* description) {
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

void Glfw::variable_update(double dt) {
    glfwPollEvents();
}

} // namespace engine::subsystems
