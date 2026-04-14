#pragma once

#include "Window.hpp"
#include "InputManager.hpp"

namespace mylib
{

class Application
{
public:
    Application();
    Application(int width, int height, std::string name);
    ~Application();

    constexpr inline mylib::Window& getWindow() { return m_window; }
    constexpr inline mylib::InputManager& getInputManager() { return m_inputManager; }
private:
    mylib::Window m_window;
    mylib::InputManager m_inputManager;
};

} //namespace mylib