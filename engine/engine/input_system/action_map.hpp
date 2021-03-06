#ifndef ENGINE_INPUT_SYSTEM_ACTION_MAP_HPP
#define ENGINE_INPUT_SYSTEM_ACTION_MAP_HPP

#include "engine/array_view.hpp"
#include "engine/input_system/input_types.hpp"

#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <variant>

namespace engine {

//! Inputs are mapped to actions which have handlers. There are multiple types
//! of handlers. The raw handler receives the type of event that occurred e.g.
//! whether it is a digital press/release or an analog event like mouse/joystick
//! movement. The raw handler also takes an array of doubles corresponding to
//! the axes of an analog event like the x and y delta of mouse movement. This
//! array is empty for digital events. A user of this class can provide a raw
//! handler to an action or they can provide a more specialized handler to an
//! action for different types of input events. These specialized handlers are
//! only called when an action is generated by a specific type of input. A
//! single input is always mapped to a single action which is mapped to a single
//! handler. Multiple inputs can be mapped to the same action like multiple
//! actions can be mapped to the same handler. Handlers can consume input by
//! returning true and pass it on by returning false, however, mouse movement is
//! always consumed if the action map below the current action map has a
//! different setting for mouse capture.
class Action_map {
public:
    using Raw_handler = std::function<bool(input::Event, Array_view<double>)>;
    using Digital_handler = std::function<bool(input::Event)>;
    using Single_axis_handler = std::function<bool(double)>;
    using Dual_axes_handler = std::function<bool(double, double)>;

    void register_binding(input::Key input, std::string action);
    void deregister_binding(input::Key input);

    void register_binding(input::Mouse input, std::string action);
    void deregister_binding(input::Mouse input);

    void register_handler(std::string action, Raw_handler handler);
    void register_handler(std::string action, Digital_handler handler);
    void register_handler(std::string action, Single_axis_handler handler);
    void register_handler(std::string action, Dual_axes_handler handler);
    void deregister_handler(const std::string& action);

    bool captures_mouse() const noexcept { return captures_mouse_; }
    void set_captures_mouse(bool flag) noexcept { captures_mouse_ = flag; }

    bool consumes_unhandled() const noexcept { return consumes_unhandled_; }
    void set_consumes_unhandled(bool flag) noexcept {
        consumes_unhandled_ = flag;
    }

    //! Returns true if a handler consumes the input, false otherwise.
    bool process_input(input::Key input, input::Event event) noexcept;
    bool process_input(input::Mouse input, input::Event event, double x,
                       double y) noexcept;

private:
    using Handler = std::variant<Raw_handler, Digital_handler,
                                 Single_axis_handler, Dual_axes_handler>;

    std::map<input::Key, std::string> key_action_map_;
    std::map<input::Mouse, std::string> mouse_action_map_;
    std::map<std::string, Handler> action_handlers_;

    bool captures_mouse_ = false;
    bool consumes_unhandled_ = false;
};

} // namespace engine

#endif /* ENGINE_INPUT_SYSTEM_ACTION_MAP_HPP */