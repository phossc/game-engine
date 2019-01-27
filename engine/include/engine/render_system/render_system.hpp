#ifndef ENGINE_RENDER_SYSTEM_RENDER_SYSTEM_HPP
#define ENGINE_RENDER_SYSTEM_RENDER_SYSTEM_HPP

#include "engine/core/component.hpp"
#include "engine/core/update_system.hpp"
#include "engine/window_manager/window.hpp"

#include <cstdint>
#include <map>

namespace engine {

//! Each renderer is categorized making it possible for view ports to only have
//! selected categories of renderers draw to it. A renderer can be part of
//! multiple categories. The priority determines the order of which the
//! renderers draw. Lower priorities are drawn first.
struct Renderer {
    using category_type = std::int32_t;
    using priority_type = std::int32_t;

    enum class Category : category_type {
        game_world = 1 << 0,
        game_ui = 1 << 1,
        overlay = 1 << 2
    };

    enum class Priority : priority_type {
        game_world = 1000,
        game_ui = 1100,
        overlay = 1'000'000'000
    };

    virtual void render(double dt) = 0;
    virtual category_type category() = 0;
    virtual priority_type priority() = 0;
};

//! This class is a system component that renderers depend on and register with.
//! The render system will call the registered renderers' render() function in
//! the order based on their priorities. Priorities of each registered renderer
//! must be unique and a renderer cannot be registered more than once at a time.
class Render_system final : public core::Component, public core::Updatable {
public:
    COMPONENT("48fbf924-8942-41cd-a096-413f4c61f7f1");
    DEPENDENCIES(Window::s_uuid());

    void activate() override;
    void deactivate() override;

    void variable_update(double dt) override;

    void register_renderer(Renderer* renderer);
    void deregister_renderer(Renderer* renderer);

private:
    std::map<Renderer::priority_type, Renderer*> renderers_;
};

} // namespace engine

#endif /* ENGINE_RENDER_SYSTEM_RENDER_SYSTEM_HPP */