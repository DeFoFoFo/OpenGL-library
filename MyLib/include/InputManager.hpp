#pragma once

#include <unordered_set>

#include "Window.hpp"

namespace mylib
{

enum class Key : GLint
{
    UNKNOWN     = GLFW_KEY_UNKNOWN,
    ESCAPE      = GLFW_KEY_ESCAPE,
    F1          = GLFW_KEY_F1,
    F2          = GLFW_KEY_F2,
    F3          = GLFW_KEY_F3,
    F4          = GLFW_KEY_F4,
    F5          = GLFW_KEY_F5,
    F6          = GLFW_KEY_F6,
    F7          = GLFW_KEY_F7,
    F8          = GLFW_KEY_F8,
    F9          = GLFW_KEY_F9,
    F10         = GLFW_KEY_F10,
    F11         = GLFW_KEY_F11,
    F12         = GLFW_KEY_F12,
    NUM1        = GLFW_KEY_1,
    NUM2        = GLFW_KEY_2,
    NUM3        = GLFW_KEY_3,
    NUM4        = GLFW_KEY_4,
    NUM5        = GLFW_KEY_5,
    NUM6        = GLFW_KEY_6,
    NUM7        = GLFW_KEY_7,
    NUM8        = GLFW_KEY_8,
    NUM9        = GLFW_KEY_9,
    NUM0        = GLFW_KEY_0,
    TAB         = GLFW_KEY_TAB,
    CAPS_LOCK   = GLFW_KEY_CAPS_LOCK,
    LEFT_SHIFT  = GLFW_KEY_LEFT_SHIFT,
    RIGHT_SHIFT = GLFW_KEY_RIGHT_CONTROL,
    LEFT_CTRL   = GLFW_KEY_LEFT_CONTROL,
    RIGHT_CTRL  = GLFW_KEY_RIGHT_CONTROL,
    LEFT_ALT    = GLFW_KEY_LEFT_ALT,
    RIGHT_ALT   = GLFW_KEY_RIGHT_ALT,
    LEFT        = GLFW_KEY_LEFT,
    RIGHT       = GLFW_KEY_RIGHT,
    UP          = GLFW_KEY_UP,
    DOWN        = GLFW_KEY_DOWN,
    ENTER       = GLFW_KEY_ENTER,
    BACKSPACE   = GLFW_KEY_BACKSPACE,
    DELETE      = GLFW_KEY_DELETE,
    INSERT      = GLFW_KEY_INSERT,
    PAGE_UP     = GLFW_KEY_PAGE_UP,
    PAGE_DOWN   = GLFW_KEY_PAGE_DOWN,
    Q           = GLFW_KEY_Q,
    W           = GLFW_KEY_W,
    R           = GLFW_KEY_R,
    T           = GLFW_KEY_T,
    Z           = GLFW_KEY_Z,
    U           = GLFW_KEY_U,
    I           = GLFW_KEY_I,
    O           = GLFW_KEY_O,
    P           = GLFW_KEY_P,
    A           = GLFW_KEY_A,
    S           = GLFW_KEY_S,
    D           = GLFW_KEY_D,
    F           = GLFW_KEY_F,
    G           = GLFW_KEY_G,
    H           = GLFW_KEY_H,
    J           = GLFW_KEY_J,
    K           = GLFW_KEY_K,
    L           = GLFW_KEY_L,
    Y           = GLFW_KEY_Y,
    X           = GLFW_KEY_X,
    C           = GLFW_KEY_C,
    V           = GLFW_KEY_V,
    B           = GLFW_KEY_B,
    N           = GLFW_KEY_N,
    M           = GLFW_KEY_M,
    SPACE       = GLFW_KEY_SPACE,
};

class InputManager
{
public:
    InputManager();
    InputManager(mylib::Window* window);

    void connectWindow(mylib::Window* window);

    bool isPressed(Key key);
    bool isJustPressed(Key key);
    bool isReleased(Key key);
    bool isJustReleased(Key key);

    inline constexpr mylib::Window* getWindow() const { return m_window; }
private:
    mylib::Window* m_window;
    std::unordered_set<GLint> m_currentKeys;

    constexpr inline GLint toGLFW(mylib::Key key) const { return static_cast<GLint>(key); }
};

} // namespace mylib