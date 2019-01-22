#include "engine/subsystems/window.hpp"

#include "engine/core/system.hpp"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

#include <cassert>

namespace engine {

using engine::core::sys;

static void window_close_callback(GLFWwindow* window) {
    assert(window == sys->component<Window>()->handle());
    sys->game_loop().stop();
}

static void framebuffer_size_callback(GLFWwindow* window, int fb_width,
                                      int fb_height) {
    assert(window == sys->component<Window>()->handle());
    glViewport(0, 0, fb_width, fb_height);
}

void Window::activate() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

    window_ = glfwCreateWindow(1280, 720, "Main window", NULL, NULL);
    glfwMakeContextCurrent(window_);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    int fb_width = 0, fb_height = 0;
    glfwGetFramebufferSize(window_, &fb_width, &fb_height);
    glViewport(0, 0, fb_width, fb_height);

    sys->logger().info("Framebuffer width: {} height: {}", fb_width, fb_height);

    glfwSetWindowCloseCallback(window_, window_close_callback);
    glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
}

void Window::deactivate() {
    glfwDestroyWindow(window_);
}

void Window::swap_buffers() {
    glfwSwapBuffers(window_);
}

void Window::set_size(int width, int height) noexcept {
    if (width <= 0 || height <= 0) {
        sys->logger().debug("Invalid window size - width: {} height: {}", width,
                            height);
        return;
    }

    glfwSetWindowSize(window_, width, height);
}

int Window::width() const noexcept {
    int width = 0;
    glfwGetWindowSize(window_, &width, nullptr);
    return width;
}

int Window::height() const noexcept {
    int height = 0;
    glfwGetWindowSize(window_, nullptr, &height);
    return height;
}

int Window::fb_width() const noexcept {
    int width = 0;
    glfwGetFramebufferSize(window_, &width, nullptr);
    return width;
}

int Window::fb_height() const noexcept {
    int height = 0;
    glfwGetFramebufferSize(window_, nullptr, &height);
    return height;
}

double Window::aspect_ratio() const noexcept {
    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(window_, &width, &height);
    return 1.0 * width / height;
}

} // namespace engine