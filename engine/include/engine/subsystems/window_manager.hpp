#ifndef ENGINE_SUBSYSTEMS_WINDOW_MANAGER_HPP
#define ENGINE_SUBSYSTEMS_WINDOW_MANAGER_HPP

#include <engine/core/component.hpp>
#include <engine/subsystems/glfw.hpp>

extern "C" struct GLFWwindow;

namespace engine {

class Window_manager : public core::Component {
public:
    COMPONENT("0cf717a0-a5d6-4915-a907-8b2210ccb353");
    DEPENDENCIES(Glfw::s_uuid());

    void create_window(int width, int height);
    GLFWwindow* window() const noexcept { return window_; }

    void swap_buffers();

private:
    GLFWwindow* window_ = nullptr;
};

} // namespace engine

#endif /* ENGINE_SUBSYSTEMS_WINDOW_MANAGER_HPP */