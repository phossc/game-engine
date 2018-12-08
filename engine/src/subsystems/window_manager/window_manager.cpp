#include <engine/subsystems/window_manager.hpp>

#include <cassert>

#include <GLFW/glfw3.h>
#include <engine/core/system.hpp>
#include <glad/glad.h>

namespace engine::subsystems {

static void window_close_callback(GLFWwindow* window) {
    core::sys->game_loop().stop();
}

void Window_manager::create_window(int width, int height) {
    // Only allow being called once at the moment.
    assert(window_ == nullptr);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    window_ = glfwCreateWindow(width, height, "Engine window", NULL, NULL);
    glfwMakeContextCurrent(window_);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    int fb_width = 0, fb_height = 0;
    glfwGetFramebufferSize(window_, &fb_width, &fb_height);
    glViewport(0, 0, fb_width, fb_height);

    glfwSetWindowUserPointer(window_, this);
    glfwSetWindowCloseCallback(window_, window_close_callback);
}

void Window_manager::swap_buffers() {
    glfwSwapBuffers(window_);
}

} // namespace engine::subsystems