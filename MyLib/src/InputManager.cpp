#include "InputManager.hpp"
#include <stdexcept>
#include <iostream>

mylib::InputManager::InputManager()
    : m_window{ nullptr }
{
}

mylib::InputManager::InputManager(mylib::Window* window)
    : m_window{ nullptr }
{
    connectWindow(window);
}

void mylib::InputManager::connectWindow(mylib::Window* window)
{
    if (window)
        m_window = window;
    else
    {
        std::cerr << "MYLIB::ERROR::INPUT_MANAGER::WINDOW_IS_NULL" << std::endl;
        throw std::runtime_error("Provided window pointer is null");
    }
}

bool mylib::InputManager::isPressed(Key key)
{
    bool state = glfwGetKey(m_window->getHandle(), toGLFW(key)) == GLFW_PRESS;
    return state;
}

bool mylib::InputManager::isJustPressed(Key key)
{
    bool state = isPressed(key);

    // If the key is not pressed, we look if it has just been released
    if (!state)
    {
        auto it = m_currentKeys.find(toGLFW(key));
        if (it != m_currentKeys.end()) // Key was not pressed last function call
            m_currentKeys.erase(it);

        return false;
    }

    auto it = m_currentKeys.find(toGLFW(key));
    if (it != m_currentKeys.end()) // Key was pressed last function call
        return false;

#ifdef MYLIB_DEBUG
    std::cout << "MYLIB::KEY_PRESSED: " << toGLFW(key) << std::endl;
#endif

    // Key was not pressed last function call
    m_currentKeys.insert(toGLFW(key));
    return true;
}

bool mylib::InputManager::isReleased(Key key)
{
    bool state = glfwGetKey(m_window->getHandle(), toGLFW(key)) == GLFW_RELEASE;
    return state;
}

bool mylib::InputManager::isJustReleased(Key key)
{
    bool state = isPressed(key);

    // If the key is not pressed, we look if it has just been released
    if (!state)
    {
        auto it = m_currentKeys.find(toGLFW(key));
        if (it != m_currentKeys.end()) // Key was not pressed last function call
        {
#ifdef MYLIB_DEBUG
            std::cout << "MYLIB::KEY_RELEASED: " << toGLFW(key) << std::endl;
#endif

            m_currentKeys.erase(it);
            return true;
        }

        return false;
    }

    auto it = m_currentKeys.find(toGLFW(key));
    if (it != m_currentKeys.end()) // Key was pressed last function call
        return false;

    // Key was not pressed last function call
    m_currentKeys.insert(toGLFW(key));
    return false;
}
