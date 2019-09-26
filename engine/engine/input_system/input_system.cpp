#include "engine/input_system/input_system.hpp"

#include "engine/core/system.hpp"
#include "engine/update_priorities.hpp"

#include "GLFW/glfw3.h"

#include <algorithm>
#include <functional>

namespace engine {

// Calls f with every action map until f returns true indicating that input
// should not be passed on to action maps further down the stack.
static void process_until_consumed(std::vector<Action_map>& action_maps,
                                   std::function<bool(Action_map&)> f) {
    auto first = std::rbegin(action_maps);
    auto last = std::rend(action_maps);
    for (; first != last; ++first) {
        if (f(*first)) {
            break;
        }
    }
}

static void key_callback(GLFWwindow*, int key, int, int action, int) {
    auto input_system = sys->component<Input_system>();

    auto input_key = static_cast<input::Key>(key);
    input::Event input_event;

    switch (action) {
    case GLFW_PRESS:
        input_event = input::Event::press;
        break;

    case GLFW_RELEASE:
        input_event = input::Event::release;
        break;

    default:
        return;
    }

    process_until_consumed(input_system->action_maps(), [&](auto& am) {
        return am.process_input(input_key, input_event);
    });
}

static void mouse_button_callback(GLFWwindow* window, int button, int action,
                                  int) {
    auto input_system = sys->component<Input_system>();
    bool captures_mouse =
            glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;

    auto input_button = static_cast<input::Mouse>(button);
    input::Event input_event;

    switch (action) {
    case GLFW_PRESS:
        input_event = input::Event::press;
        break;

    case GLFW_RELEASE:
        input_event = input::Event::release;
        break;

    default:
        return;
    }

    process_until_consumed(input_system->action_maps(), [&](auto& am) {
        if (am.captures_mouse() == captures_mouse) {
            return am.process_input(input_button, input_event, 0.0, 0.0);
        }
        else {
            return true;
        }
    });
}

static void cursor_position_callback(GLFWwindow* window, double x, double y) {
    auto input_system = sys->component<Input_system>();
    bool captures_mouse =
            glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;

    process_until_consumed(input_system->action_maps(), [&](auto& am) {
        if (am.captures_mouse() == captures_mouse) {
            return am.process_input(input::Mouse::delta_xy,
                                    input::Event::dual_axes, x, y);
        }
        else {
            return true;
        }
    });
}

void Input_system::push_action_map(Action_map action_map) {
    action_maps_.emplace_back(std::move(action_map));
}

Action_map Input_system::pop_action_map() {
    if (action_maps_.empty()) {
        sys->logger().critical("Removing an action map when there is "
                               "none is undefined behavior");
    }

    auto action_map = std::move(action_maps_.back());
    action_maps_.pop_back();
    return action_map;
}

Action_map& Input_system::current_action_map() noexcept {
    if (action_maps_.empty()) {
        sys->logger().critical("Retrieving the current action map when "
                               "there is none is undefined behavior");
    }

    return action_maps_.back();
}

const std::vector<Action_map>& Input_system::action_maps() const noexcept {
    return action_maps_;
}

std::vector<Action_map>& Input_system::action_maps() noexcept {
    return action_maps_;
}

void Input_system::activate() {
    sys->update_system().register_variable_update(
            this, static_cast<std::int32_t>(Update_priority::input));

    auto window = dependency<Window>()->handle();
    assert(window != nullptr);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
}

void Input_system::deactivate() {
    sys->update_system().deregister_variable_update(this);

    auto window = dependency<Window>()->handle();
    assert(window != nullptr);
    glfwSetKeyCallback(window, nullptr);
    glfwSetMouseButtonCallback(window, nullptr);
    glfwSetCursorPosCallback(window, nullptr);
}

void Input_system::variable_update(double) {
    auto window = dependency<Window>()->handle();
    int cursor_value = glfwGetInputMode(window, GLFW_CURSOR);
    bool captures_mouse = cursor_value == GLFW_CURSOR_DISABLED;
    if (!action_maps_.empty() &&
        current_action_map().captures_mouse() != captures_mouse) {
        cursor_value = cursor_value == GLFW_CURSOR_DISABLED
                               ? GLFW_CURSOR_NORMAL
                               : GLFW_CURSOR_DISABLED;
        glfwSetInputMode(window, GLFW_CURSOR, cursor_value);
    }
}

} // namespace engine