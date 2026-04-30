#include "Shader.hpp"

#include "GLFW/glfw3.h"

#include "Defines.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

mylib::Shader::Shader()
    : m_ID{}
{
}

mylib::Shader::Shader(Shader&& other) noexcept
    : m_ID{ other.m_ID }, m_uniforms{ other.m_uniforms }, m_fileNames{ other.m_fileNames }
{
    other.m_ID = 0;
    other.m_uniforms.clear();
    other.m_fileNames.clear();
}

mylib::Shader& mylib::Shader::operator=(Shader&& other) noexcept
{
    if (this != &other) {
        if (m_ID)
            glDeleteProgram(m_ID);
        m_ID = other.m_ID;
        m_uniforms = other.m_uniforms;
        m_fileNames = other.m_fileNames;
        other.m_ID = 0;
        other.m_uniforms.clear();
        other.m_fileNames.clear();
    }
    return *this;
}

mylib::Shader::~Shader()
{
    if (m_ID != 0) {
        glDeleteProgram(m_ID);
    }
}

// Reads a file and returns its content
std::string mylib::Shader::readFile(const std::string& filePath)
{
    std::ifstream file{ filePath };
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    if (file) {
        std::stringstream ss;
        ss << file.rdbuf();
        file.close();
        return ss.str();
    }
    else {
        std::cerr << "MYLIB::ERROR::SHADER::COULD_NOT_READ_FILE\tPATH: \""
            << filePath << "\"" << std::endl;
    }
    return "";
}

void mylib::Shader::checkCompileStatus(GLuint shader, const std::string& filePath)
{
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "MYLIB::ERROR::SHADER::COMPILATION_FAILED\tPATH: \""
            << filePath << "\"\n"
            << infoLog << std::endl;
    }
}

void mylib::Shader::checkLinkStatus()
{
    int success;
    char infoLog[512];
    glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
        std::cerr << "MYLIB::ERROR::SHADER::LINKING_FAILED\nPATHS: ";

        for (const auto& path : m_fileNames)
        {
            std::cerr << "\t\"" << path << "\"\n";
        }

        std::cerr << infoLog << std::endl;
    }
}

GLint mylib::Shader::getUniformLocation(const std::string& name)
{
    auto it = m_uniforms.find(name);
    if (it != m_uniforms.end())
        return it->second;

    GLint location = glGetUniformLocation(m_ID, name.c_str());
#if DEBUG == true
    if (location == -1)
        std::cerr << "MYLIB::ERROR::SHADER::UNIFORM_NOT_FOUND: " << name
        << std::endl;
#endif

    m_uniforms.insert({ name, location });
    return location;
}