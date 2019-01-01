#ifndef ENGINE_SUBSYSTEMS_WINDOW_HPP
#define ENGINE_SUBSYSTEMS_WINDOW_HPP

#include <engine/core/component.hpp>
#include <engine/subsystems/glfw.hpp>

extern "C" struct GLFWwindow;

namespace engine {

class Window : public core::Component {
public:
    COMPONENT("0cf717a0-a5d6-4915-a907-8b2210ccb353");
    DEPENDENCIES(Glfw::s_uuid());

    void activate() override;
    void deactivate() override;

    void swap_buffers();

    //! Changes both window and framebuffer sizes. The window is set to the
    //! requested size and the framebuffer is set based on the requested size
    //! and the current scale of the monitor.
    void set_size(int width, int height) noexcept;
    int width() const noexcept;
    int height() const noexcept;

    //! Framebuffer size related methods.
    int fb_width() const noexcept;
    int fb_height() const noexcept;

    //! Ratio between the width and height of the framebuffer.
    double aspect_ratio() const noexcept;

    //! Underlying GLFW handle to the window.
    GLFWwindow* handle() const noexcept { return window_; }

private:
    GLFWwindow* window_ = nullptr;
};

} // namespace engine

#endif /* ENGINE_SUBSYSTEMS_WINDOW_HPP */