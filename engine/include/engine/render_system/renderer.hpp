#ifndef RENDER_SYSTEM_RENDERERS_RENDERER_HPP
#define RENDER_SYSTEM_RENDERERS_RENDERER_HPP

#include <engine/core/component.hpp>
#include <engine/core/system.hpp>

class Renderer : public engine::core::Component,
                 public engine::core::Updatable {
public:
    COMPONENT("c975a4b6-bd5c-46fc-b359-04ccb4a5ba8c");
    DEPENDENCIES();

    virtual void render() = 0;
    virtual std::int32_t priority() = 0;

    void activate() override {
        using engine::core::sys;
        sys->update_system().register_variable_update(this, priority());
    }

    void deactivate() override {
        using engine::core::sys;
        sys->update_system().deregister_variable_update(this);
    }

    void variable_update(double) override { render(); }
};

#endif /* RENDER_SYSTEM_RENDERERS_RENDERER_HPP */