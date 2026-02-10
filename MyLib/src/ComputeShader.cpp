#include "ComputeShader.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

mylib::ComputeShader::ComputeShader()
    : m_ID{0}
{}

// Expects a filepath to the compute shader
mylib::ComputeShader::ComputeShader(const char* filePath)
    : m_ID{0}
{
    assign(filePath);
}

mylib::ComputeShader::~ComputeShader()
{
    if (m_ID != 0)
    {
        glDeleteProgram(m_ID);
    }
}

// Expects a filepath to the compute shader
void mylib::ComputeShader::assign(const char* filePath)
{
    GLint versionMajor = 0, versionMinor = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
    glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
    if ((versionMajor < 4) || (versionMajor >= 4 && versionMinor < 3))
    {
        std::cerr << "MYLIB::ERROR::COMPUTE_SHADER::OPENGL_VERSION_LESS_THAN_4.3\tCURRENT: " << versionMajor << "." << versionMinor << std::endl;
    }

    std::string code{readFile(filePath)};
    
    const char* sourceCode = code.c_str();
    GLuint compute = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(compute, 1, &sourceCode, NULL);

    glCompileShader(compute);
    checkCompileStatus(compute, filePath);

    m_ID = glCreateProgram();
    glAttachShader(m_ID, compute);
    glLinkProgram(m_ID);
    glDeleteShader(compute);

    checkLinkStatus(filePath);
}

// Returns the program ID
GLuint mylib::ComputeShader::getID()
{
    return m_ID;
}

// Reads a file and returns its content
std::string mylib::ComputeShader::readFile(const char* filePath)
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
        std::cerr << "MYLIB::ERROR::COMPUTE_SHADER::COULD_NOT_READ_FILE\tPATH: \"" << filePath << "\"" << std::endl;
    }
    return "";
}

void mylib::ComputeShader::checkCompileStatus(GLuint shader, const char* filePath)
{
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "MYLIB::ERROR::COMPUTE_SHADER::COMPILATION_FAILED\tPATH: \"" << filePath << "\"\n" << infoLog << std::endl;
    }
}

void mylib::ComputeShader::checkLinkStatus(const char* filePath)
{
    int success;
    char infoLog[512];
    glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
        std::cerr << "MYLIB::ERROR::COMPUTE_SHADER::LINKING_FAILED\tPATH: \"" << filePath << "\"\n" << infoLog << std::endl;
    }
}