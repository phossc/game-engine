#ifndef ENGINE_INPUT_SYSTEM_INPUT_TYPES_HPP
#define ENGINE_INPUT_SYSTEM_INPUT_TYPES_HPP

#include "GLFW/glfw3.h"

namespace engine::input {

enum class Event { release = GLFW_RELEASE, press = GLFW_PRESS, single_axis = -1, dual_axes = -2 };

enum class Mouse {
    primary = GLFW_MOUSE_BUTTON_LEFT,
    secondary = GLFW_MOUSE_BUTTON_RIGHT,
    middle = GLFW_MOUSE_BUTTON_MIDDLE,
    four = GLFW_MOUSE_BUTTON_4,
    five = GLFW_MOUSE_BUTTON_5,
    six = GLFW_MOUSE_BUTTON_6,
    seven = GLFW_MOUSE_BUTTON_7,
    eight = GLFW_MOUSE_BUTTON_8,
    delta_xy = -1
};

enum class Key {
    unknown = GLFW_KEY_UNKNOWN,

    arrow_down = GLFW_KEY_DOWN,
    arrow_left = GLFW_KEY_LEFT,
    arrow_right = GLFW_KEY_RIGHT,
    arrow_up = GLFW_KEY_UP,
    backspace = GLFW_KEY_BACKSPACE,
    caps_lock = GLFW_KEY_CAPS_LOCK,
    del = GLFW_KEY_DELETE,
    end = GLFW_KEY_END,
    enter = GLFW_KEY_ENTER,
    escape = GLFW_KEY_ESCAPE,
    home = GLFW_KEY_HOME,
    insert = GLFW_KEY_INSERT,
    menu = GLFW_KEY_MENU,
    num_lock = GLFW_KEY_NUM_LOCK,
    page_down = GLFW_KEY_PAGE_DOWN,
    page_up = GLFW_KEY_PAGE_UP,
    pause = GLFW_KEY_PAUSE,
    print_screen = GLFW_KEY_PRINT_SCREEN,
    scroll_lock = GLFW_KEY_SCROLL_LOCK,
    tab = GLFW_KEY_TAB,

    left_alt = GLFW_KEY_LEFT_ALT,
    left_control = GLFW_KEY_LEFT_CONTROL,
    left_shift = GLFW_KEY_LEFT_SHIFT,
    left_super = GLFW_KEY_LEFT_SUPER,
    right_alt = GLFW_KEY_RIGHT_ALT,
    right_control = GLFW_KEY_RIGHT_CONTROL,
    right_shift = GLFW_KEY_RIGHT_SHIFT,
    right_super = GLFW_KEY_RIGHT_SUPER,

    f1 = GLFW_KEY_F1,
    f2 = GLFW_KEY_F2,
    f3 = GLFW_KEY_F3,
    f4 = GLFW_KEY_F4,
    f5 = GLFW_KEY_F5,
    f6 = GLFW_KEY_F6,
    f7 = GLFW_KEY_F7,
    f8 = GLFW_KEY_F8,
    f9 = GLFW_KEY_F9,
    f10 = GLFW_KEY_F10,
    f11 = GLFW_KEY_F11,
    f12 = GLFW_KEY_F12,
    f13 = GLFW_KEY_F13,
    f14 = GLFW_KEY_F14,
    f15 = GLFW_KEY_F15,
    f16 = GLFW_KEY_F16,
    f17 = GLFW_KEY_F17,
    f18 = GLFW_KEY_F18,
    f19 = GLFW_KEY_F19,
    f20 = GLFW_KEY_F20,
    f21 = GLFW_KEY_F21,
    f22 = GLFW_KEY_F22,
    f23 = GLFW_KEY_F23,
    f24 = GLFW_KEY_F24,
    f25 = GLFW_KEY_F25,

    apostrophe = GLFW_KEY_APOSTROPHE,
    backslash = GLFW_KEY_BACKSLASH,
    comma = GLFW_KEY_COMMA,
    dash = GLFW_KEY_MINUS,
    equal = GLFW_KEY_EQUAL,
    grave_accent = GLFW_KEY_GRAVE_ACCENT,
    left_bracket = GLFW_KEY_LEFT_BRACKET,
    period = GLFW_KEY_PERIOD,
    right_bracket = GLFW_KEY_RIGHT_BRACKET,
    semicolon = GLFW_KEY_SEMICOLON,
    slash = GLFW_KEY_SLASH,
    space = GLFW_KEY_SPACE,
    world_1 = GLFW_KEY_WORLD_1,
    world_2 = GLFW_KEY_WORLD_2,

    a = GLFW_KEY_A,
    b = GLFW_KEY_B,
    c = GLFW_KEY_C,
    d = GLFW_KEY_D,
    e = GLFW_KEY_E,
    f = GLFW_KEY_F,
    g = GLFW_KEY_G,
    h = GLFW_KEY_H,
    i = GLFW_KEY_I,
    j = GLFW_KEY_J,
    k = GLFW_KEY_K,
    l = GLFW_KEY_L,
    m = GLFW_KEY_M,
    n = GLFW_KEY_N,
    o = GLFW_KEY_O,
    p = GLFW_KEY_P,
    q = GLFW_KEY_Q,
    r = GLFW_KEY_R,
    s = GLFW_KEY_S,
    t = GLFW_KEY_T,
    u = GLFW_KEY_U,
    v = GLFW_KEY_V,
    w = GLFW_KEY_W,
    x = GLFW_KEY_X,
    y = GLFW_KEY_Y,
    z = GLFW_KEY_Z,

    numrow_0 = GLFW_KEY_0,
    numrow_1 = GLFW_KEY_1,
    numrow_2 = GLFW_KEY_2,
    numrow_3 = GLFW_KEY_3,
    numrow_4 = GLFW_KEY_4,
    numrow_5 = GLFW_KEY_5,
    numrow_6 = GLFW_KEY_6,
    numrow_7 = GLFW_KEY_7,
    numrow_8 = GLFW_KEY_8,
    numrow_9 = GLFW_KEY_9,

    numpad_0 = GLFW_KEY_KP_0,
    numpad_1 = GLFW_KEY_KP_1,
    numpad_2 = GLFW_KEY_KP_2,
    numpad_3 = GLFW_KEY_KP_3,
    numpad_4 = GLFW_KEY_KP_4,
    numpad_5 = GLFW_KEY_KP_5,
    numpad_6 = GLFW_KEY_KP_6,
    numpad_7 = GLFW_KEY_KP_7,
    numpad_8 = GLFW_KEY_KP_8,
    numpad_9 = GLFW_KEY_KP_9,
    numpad_add = GLFW_KEY_KP_ADD,
    numpad_decimal = GLFW_KEY_KP_DECIMAL,
    numpad_divide = GLFW_KEY_KP_DIVIDE,
    numpad_enter = GLFW_KEY_KP_ENTER,
    numpad_equal = GLFW_KEY_KP_EQUAL,
    numpad_multiply = GLFW_KEY_KP_MULTIPLY,
    numpad_subtract = GLFW_KEY_KP_SUBTRACT
};

} // namespace engine::input

#endif /* ENGINE_INPUT_SYSTEM_INPUT_TYPES_HPP */