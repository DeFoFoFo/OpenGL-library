#pragma once

#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include <string_view>
#include <string>

namespace mylib
{

class Window
{
public:
    Window();
    Window(const int32_t width, const int32_t height, const std::string_view name, bool fullscreen = false);
    ~Window();
    Window(const Window& source) = delete;
    Window& operator=(const Window& source) = delete;
    Window(Window&& source) noexcept;
    Window& operator=(Window&& source) noexcept;

    void createWindow(const int32_t width, const int32_t height, const std::string_view name, bool fullscreen = false);
    void setFullScreen(bool state);

    inline void setCurrentContext() {glfwMakeContextCurrent(m_handle);}

    constexpr inline GLFWwindow* getHandle() const {return m_handle;}
    constexpr inline std::string getName() const {return m_name;}
    constexpr inline int32_t getWidth() const {return m_width;}
    constexpr inline int32_t getHeight() const {return m_height;}
private:
    GLFWwindow* m_handle;
    std::string m_name;
    int32_t m_width;
    int32_t m_height;

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};

} // namespace mylib