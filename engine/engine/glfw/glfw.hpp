#ifndef ENGINE_GLFW_GLFW_HPP
#define ENGINE_GLFW_GLFW_HPP

#include "engine/core/component.hpp"
#include "engine/core/update_system.hpp"

namespace engine {

class Glfw final : public Component, public Updatable {
public:
    COMPONENT("1757336f-d5de-47a4-8f7b-e961b37974d4");
    DEPENDENCIES();

    void activate() override;
    void deactivate() override;

    void variable_update(double dt) override;
};

} // namespace engine

#endif /* ENGINE_GLFW_GLFW_HPP */