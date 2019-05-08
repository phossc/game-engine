#ifndef ENGINE_INPUT_SYSTEM_INPUT_SYSTEM_HPP
#define ENGINE_INPUT_SYSTEM_INPUT_SYSTEM_HPP

#include "engine/core/component.hpp"
#include "engine/core/update_system.hpp"
#include "engine/glfw/glfw.hpp"
#include "engine/input_system/action_map.hpp"
#include "engine/window_manager/window.hpp"

#include <vector>

namespace engine {

class Input_system : public core::Component, public core::Updatable {
public:
    COMPONENT("9cbc73de-f5ac-4d04-a18b-775b15889de2");
    DEPENDENCIES(Window::uuid_s());

    void push_action_map(Action_map action_map);
    Action_map pop_action_map();
    Action_map& current_action_map() noexcept;
    const std::vector<Action_map>& action_maps() const noexcept;
    std::vector<Action_map>& action_maps() noexcept;

    void activate() override;
    void deactivate() override;

    void variable_update(double) override;

private:
    std::vector<Action_map> action_maps_;
};

} // namespace engine

#endif /* ENGINE_INPUT_SYSTEM_INPUT_SYSTEM_HPP */