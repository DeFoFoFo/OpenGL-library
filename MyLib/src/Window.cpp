#include "Window.hpp"

#include "Defines.hpp"

#include <iostream>

mylib::Window::Window()
    : m_handle{nullptr}, m_width{0}, m_height{0}
{}

mylib::Window::Window(const int32_t width, const int32_t height, const std::string_view name, bool fullscreen)
    : m_handle{nullptr}, m_name{name}, m_width{width}, m_height{height}
{
    createWindow(width, height, name, fullscreen);
}

mylib::Window::~Window()
{
    if (m_handle)
        glfwDestroyWindow(m_handle);
}

mylib::Window::Window(Window &&source) noexcept
    : m_handle{source.m_handle}, m_width{source.m_width}, m_height{source.m_height}
{
    source.m_handle = nullptr;
    source.m_name = "";
    source.m_width = 0;
    source.m_height = 0;
}

mylib::Window &mylib::Window::operator=(Window &&source) noexcept
{
    if (this != &source)
    {
        if (m_handle) glfwDestroyWindow(m_handle);
        m_handle = source.m_handle;
        m_name = source.m_name;
        m_width = source.m_width;
        m_height = source.m_height;
        source.m_handle = nullptr;
        source.m_name = "";
        source.m_width = 0;
        source.m_height = 0;
    }
    return *this;
}

void APIENTRY glDebugOutput(GLenum source, 
                            GLenum type, 
                            GLuint id, 
                            GLenum severity, 
                            GLsizei length, 
                            const GLchar *message, 
                            const void *userParam)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}

void mylib::Window::createWindow(const int32_t width, const int32_t height, const std::string_view name, bool fullscreen)
{
    if (m_handle)
    {
        glfwDestroyWindow(m_handle);
        m_width = width;
        m_height = height;
        m_name = name;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef MYLIB_DEBUG
#if MIN_OPENGL_VERSION(4,3)
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
#endif
    m_handle = glfwCreateWindow(width, height, name.data(), 
                                (fullscreen) ? glfwGetPrimaryMonitor() : nullptr, nullptr);
    
    if (!m_handle)
    {
        throw std::runtime_error("Failed to create window");
    }
    glfwMakeContextCurrent(m_handle);

    if (!gladLoadGL(glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize glad");
    }

#ifdef MYLIB_DEBUG
#if MIN_OPENGL_VERSION(4,3)
    int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    // Uncomment for filtering debug messages
    // glDebugMessageControl(GL_DEBUG_SOURCE_API, 
    //                       GL_DEBUG_TYPE_ERROR, 
    //                       GL_DEBUG_SEVERITY_HIGH,
    //                       0, nullptr, GL_TRUE);
#endif
#endif

    glfwSetWindowUserPointer(m_handle, this);
    glfwSetFramebufferSizeCallback(m_handle, Window::framebufferSizeCallback);
}

void mylib::Window::setFullScreen(bool state)
{
    createWindow(m_width, m_height, m_name, state);
}

void mylib::Window::framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

    // Stealing current context
    Window* prevWindow = static_cast<Window*>(glfwGetWindowUserPointer(glfwGetCurrentContext()));
    self->setCurrentContext();

    glViewport(0, 0, width, height);

    if (self) {
        self->m_width = width;
        self->m_height = height;
    }

#ifdef MYLIB_DEBUG
    std::cout << "Resized: " << self->m_width << " " << self->m_height << std::endl;
#endif

    // Restore context
    prevWindow->setCurrentContext();
}
