#include "Shader.hpp"

#include "GLFW/glfw3.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

mylib::Shader::Shader()
    : m_ID{0}
{}

// Expects a file path to the vertex shader and then the fragment shader
mylib::Shader::Shader(const char* vsFilePath, const char* fsFilePath)
    : m_ID{0}
{
    assign(vsFilePath, fsFilePath);
}

mylib::Shader::~Shader()
{
    if (m_ID != 0)
    {
        glDeleteProgram(m_ID);
    }
}

// Expects a file path to the vertex shader and then the fragment shader
void mylib::Shader::assign(const char* vsFilePath, const char* fsFilePath)
{
    std::string vsCode{readFile(vsFilePath)}, fsCode{readFile(fsFilePath)};
    
    const char* vsSourceCode = vsCode.c_str();
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vsSourceCode, NULL);

    const char* fsSourceCode = fsCode.c_str();
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fsSourceCode, NULL);

    glCompileShader(vertex);
    checkCompileStatus(vertex, vsFilePath);

    glCompileShader(fragment);
    checkCompileStatus(fragment, fsFilePath);

    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertex);
    glAttachShader(m_ID, fragment);
    glLinkProgram(m_ID);
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    checkLinkStatus(vsFilePath, fsFilePath);
}

// Returns the program ID
GLuint mylib::Shader::ID()
{
    return m_ID;
}

// Reads a file and returns its content
std::string mylib::Shader::readFile(const char* filePath)
{
    std::ifstream file{filePath};
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    if (file)
    {
        std::stringstream ss;
        ss << file.rdbuf();
        file.close();
        return ss.str();
    }
    else
    {
        std::cerr << "MYLIB::ERROR::SHADER::COULD_NOT_READ_FILE\tPATH: \"" << filePath << "\"" << std::endl;
    }
    return "";
}

void mylib::Shader::checkCompileStatus(GLuint shader, const char* filePath)
{
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "MYLIB::ERROR::SHADER::COMPILATION_FAILED\tPATH: \"" << filePath << "\"\n" << infoLog << std::endl;
    }
}

void mylib::Shader::checkLinkStatus(const char* vsFilePath, const char* fsFilePath)
{
    int success;
    char infoLog[512];
    glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
        std::cerr << "MYLIB::ERROR::SHADER::LINKING_FAILED\tVERTEX_PATH: \"" << vsFilePath
                                                    << "\"\tFRAGMENT_PATH: \"" << fsFilePath << "\"\n"
                                                    << infoLog << std::endl;
    }
}