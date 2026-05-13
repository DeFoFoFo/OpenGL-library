#include "ShaderProgram.hpp"

#include "GLFW/glfw3.h"

#include "Defines.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

mylib::ShaderProgram::ShaderProgram()
    : mylib::Shader::Shader{}
{
}

mylib::ShaderProgram::ShaderProgram(const std::string& vsFilePath, const std::string& fsFilePath)
{
    assign(vsFilePath, fsFilePath);
}

mylib::ShaderProgram::ShaderProgram(const std::string& vsFilePath, const std::string& fsFilePath, const std::string& gsFilePath)
{
    assign(vsFilePath, fsFilePath, gsFilePath);
}

mylib::ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
    : mylib::Shader(std::move(other))
{
}

mylib::ShaderProgram& mylib::ShaderProgram::operator=(ShaderProgram&& other) noexcept
{
    if (this != &other)
    {
        mylib::Shader::operator=(std::move(other));
    }
    return *this;
}

void mylib::ShaderProgram::assign(const std::string& vsFilePath, const std::string& fsFilePath)
{
#ifdef MYLIB_DEBUG
    float startTime = glfwGetTime();
#endif

    if (m_ID != 0)
    {
        glDeleteProgram(m_ID);
        m_uniforms.clear();
    }

    std::string vsCode{ readFile(vsFilePath) }, fsCode{ readFile(fsFilePath) };

    GLuint vertex{ glCreateShader(GL_VERTEX_SHADER) }, fragment{ glCreateShader(GL_FRAGMENT_SHADER) };

    const char* vsSourceCode{ vsCode.c_str() }, * fsSourceCode{ fsCode.c_str() };

    glShaderSource(vertex, 1, &vsSourceCode, NULL);
    glShaderSource(fragment, 1, &fsSourceCode, NULL);

    glCompileShader(vertex);
    glCompileShader(fragment);

    checkCompileStatus(vertex, vsFilePath);
    checkCompileStatus(fragment, fsFilePath);

    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertex);
    glAttachShader(m_ID, fragment);
    glLinkProgram(m_ID);
    checkLinkStatus();

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    m_fileNames = { vsFilePath, fsFilePath };

#ifdef MYLIB_DEBUG
    float totalTime = glfwGetTime() - startTime;
    std::cout << "MYLIB::SHADER_PROGRAM::LOADED_IN " << totalTime * 1000 << "ms\n";
    for (const auto& path : m_fileNames)
        std::cout << "\tPATH: \"" << path << "\"\n";
#endif
}

void mylib::ShaderProgram::assign(const std::string& vsFilePath, const std::string& fsFilePath, const std::string& gsFilePath)
{
#ifdef MYLIB_DEBUG
    float startTime = glfwGetTime();
#endif

    if (m_ID != 0)
    {
        glDeleteProgram(m_ID);
        m_uniforms.clear();
    }

    std::string vsCode{ readFile(vsFilePath) }, fsCode{ readFile(fsFilePath) }, gsCode{ readFile(gsFilePath) };

    GLuint vertex{ glCreateShader(GL_VERTEX_SHADER) }, fragment{ glCreateShader(GL_FRAGMENT_SHADER) }, geometry{ glCreateShader(GL_GEOMETRY_SHADER) };

    const char* vsSourceCode{ vsCode.c_str() }, * fsSourceCode{ fsCode.c_str() }, * gsSourceCode{ gsCode.c_str() };

    glShaderSource(vertex, 1, &vsSourceCode, NULL);
    glShaderSource(fragment, 1, &fsSourceCode, NULL);
    glShaderSource(geometry, 1, &gsSourceCode, NULL);

    glCompileShader(vertex);
    glCompileShader(fragment);
    glCompileShader(geometry);

    checkCompileStatus(vertex, vsFilePath);
    checkCompileStatus(fragment, fsFilePath);
    checkCompileStatus(geometry, gsFilePath);

    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertex);
    glAttachShader(m_ID, fragment);
    glAttachShader(m_ID, geometry);
    glLinkProgram(m_ID);
    checkLinkStatus();

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    glDeleteShader(geometry);

    m_fileNames = { vsFilePath, fsFilePath, gsFilePath };

#ifdef MYLIB_DEBUG
    float totalTime = glfwGetTime() - startTime;
    std::cout << "MYLIB::SHADER_PROGRAM::LOADED_IN " << totalTime * 1000 << "ms\n";
    for (const auto& path : m_fileNames)
        std::cout << "\tPATH: \"" << path << "\"\n";
#endif
}

void mylib::ShaderProgram::recompile()
{
    if (m_fileNames.size() == 2)
        assign(m_fileNames[0], m_fileNames[1]);
    else if (m_fileNames.size() == 3)
        assign(m_fileNames[0], m_fileNames[1], m_fileNames[2]);
}
