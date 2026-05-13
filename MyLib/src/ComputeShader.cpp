#include "ComputeShader.hpp"

#include "Defines.hpp"
#if MIN_OPENGL_VERSION(4,3)

#include "GLFW/glfw3.h"

#include <iostream>
#include <fstream>
#include <sstream>

mylib::ComputeShader::ComputeShader()
    : mylib::Shader::Shader{}
{
}

// Expects a filepath to the compute shader
mylib::ComputeShader::ComputeShader(const std::string& filePath)
{
    assign(filePath);
}

mylib::ComputeShader::ComputeShader(ComputeShader&& other) noexcept
    : mylib::Shader(std::move(other))
{
}

mylib::ComputeShader& mylib::ComputeShader::operator=(ComputeShader&& other) noexcept
{
    if (this != &other)
    {
        mylib::Shader::operator=(std::move(other));
    }
    return *this;
}

// Expects a filepath to the compute shader
void mylib::ComputeShader::assign(const std::string& filePath)
{
#ifdef MYLIB_DEBUG
    float startTime = glfwGetTime();
#endif

    if (m_ID != 0)
    {
        glDeleteProgram(m_ID);
        m_uniforms.clear();
    }

    std::string code{ readFile(filePath) };

    const char* sourceCode = code.c_str();
    GLuint compute = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(compute, 1, &sourceCode, NULL);

    glCompileShader(compute);
    checkCompileStatus(compute, filePath);

    m_ID = glCreateProgram();
    glAttachShader(m_ID, compute);
    glLinkProgram(m_ID);
    glDeleteShader(compute);

    checkLinkStatus();

    m_fileNames = { filePath };

#ifdef MYLIB_DEBUG
    float totalTime = glfwGetTime() - startTime;
    std::cout << "MYLIB::COMPUTE_SHADER::LOADED_IN " << totalTime * 1000 << "ms\n"
        << "\tPATH: " << m_fileNames[0] << std::endl;
#endif
}

void mylib::ComputeShader::recompile()
{
    if (!m_fileNames.empty())
        assign(m_fileNames[0]);
}

#endif