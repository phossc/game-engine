#include "engine/input_system/action_map.hpp"

namespace engine {

void Action_map::register_binding(input::Key input, std::string action) {
    key_action_map_.emplace(input, std::move(action));
}

void Action_map::deregister_binding(input::Key input) {
    key_action_map_.erase(input);
}

void Action_map::register_binding(input::Mouse input, std::string action) {
    mouse_action_map_.emplace(input, std::move(action));
}

void Action_map::deregister_binding(input::Mouse input) {
    mouse_action_map_.erase(input);
}

void Action_map::register_handler(std::string action, Raw_handler handler) {
    action_handlers_.emplace(std::move(action), handler);
}

void Action_map::register_handler(std::string action, Digital_handler handler) {
    action_handlers_.emplace(std::move(action), handler);
}

void Action_map::register_handler(std::string action, Single_axis_handler handler) {
    action_handlers_.emplace(std::move(action), handler);
}

void Action_map::register_handler(std::string action, Dual_axes_handler handler) {
    action_handlers_.emplace(std::move(action), handler);
}

void Action_map::deregister_handler(const std::string& action) {
    action_handlers_.erase(action);
}

class Handler_invoker {
public:
    Handler_invoker(input::Event event, double x, double y, bool consumes_unhandled)
        : event_(event), axes_{x, y}, consumes_unhandled_(consumes_unhandled) {}

    bool operator()(Action_map::Raw_handler& handler) {
        switch (event_) {
        case input::Event::press: // Fall through
        case input::Event::release:
            return handler(event_, {});
        case input::Event::single_axis:
            return handler(event_, {axes_, 1});
        case input::Event::dual_axes:
            return handler(event_, {axes_, 2});
        default:
            return consumes_unhandled_;
        }
    }

    bool operator()(Action_map::Digital_handler& handler) {
        switch (event_) {
        case input::Event::press: // Fall through
        case input::Event::release:
            return handler(event_);
        default:
            return consumes_unhandled_;
        }
    }

    bool operator()(Action_map::Single_axis_handler& handler) {
        switch (event_) {
        case input::Event::single_axis:
            return handler(axes_[0]);
        default:
            return consumes_unhandled_;
        }
    }

    bool operator()(Action_map::Dual_axes_handler& handler) {
        switch (event_) {
        case input::Event::dual_axes:
            return handler(axes_[0], axes_[1]);
        default:
            return consumes_unhandled_;
        }
    }

private:
    input::Event event_;
    double axes_[2];
    bool consumes_unhandled_;
};

bool Action_map::process_input(input::Key input, input::Event event) noexcept {
    auto action_iter = key_action_map_.find(input);
    if (action_iter == std::end(key_action_map_)) {
        return consumes_unhandled_;
    }

    auto handler_iter = action_handlers_.find(action_iter->second);
    if (handler_iter == std::end(action_handlers_)) {
        return consumes_unhandled_;
    }

    Handler_invoker invoker(event, 0.0, 0.0, consumes_unhandled_);
    return std::visit(invoker, handler_iter->second);
}

bool Action_map::process_input(input::Mouse input, input::Event event, double x,
                               double y) noexcept {
    auto action_iter = mouse_action_map_.find(input);
    if (action_iter == std::end(mouse_action_map_)) {
        return consumes_unhandled_;
    }

    auto handler_iter = action_handlers_.find(action_iter->second);
    if (handler_iter == std::end(action_handlers_)) {
        return consumes_unhandled_;
    }

    Handler_invoker invoker(event, x, y, consumes_unhandled_);
    return std::visit(invoker, handler_iter->second);
}

} // namespace engine