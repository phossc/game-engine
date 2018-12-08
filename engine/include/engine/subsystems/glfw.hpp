#ifndef ENGINE_SUBSYSTEMS_GLFW_HPP
#define ENGINE_SUBSYSTEMS_GLFW_HPP

#include <engine/core/component.hpp>
#include <engine/core/update_system.hpp>

namespace engine::subsystems {

class Glfw final : public core::Component,
                   public core::Updatable {
public:
    COMPONENT("1757336f-d5de-47a4-8f7b-e961b37974d4");
    DEPENDENCIES();

    void activate() override;
    void deactivate() override;

    void variable_update(double dt) override;
};

} // namespace engine::subsystems

#endif /* ENGINE_SUBSYSTEMS_GLFW_HPP */
