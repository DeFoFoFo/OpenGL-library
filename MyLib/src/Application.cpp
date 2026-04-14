#include "Application.hpp"

#include "GLFW/glfw3.h"

mylib::Application::Application()
{
    glfwInit();
}

mylib::Application::Application(int width, int height, std::string name)
{
    m_window.createWindow(width, height, name);
    m_inputManager.connectWindow(&m_window);
}

mylib::Application::~Application()
{
    glfwTerminate();
}
