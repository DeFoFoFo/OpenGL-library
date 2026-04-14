#include "ComputeShader.hpp"

#include "Defines.hpp"
#if MIN_OPENGL_VERSION(4,3)

#include <iostream>
#include <fstream>
#include <sstream>

mylib::ComputeShader::ComputeShader()
{}

// Expects a filepath to the compute shader
mylib::ComputeShader::ComputeShader(std::string filePath)
{
    assign(filePath);
}

mylib::ComputeShader::ComputeShader(ComputeShader &&other) noexcept
    : m_ID{other.m_ID}, m_uniforms{other.m_uniforms}
{
    other.m_ID = 0;
    other.m_uniforms.clear();
}

mylib::ComputeShader &mylib::ComputeShader::operator=(ComputeShader &&other) noexcept
{
    if (this != &other)
    {
        if (m_ID) glDeleteProgram(m_ID);
        m_ID = other.m_ID;
        m_uniforms = other.m_uniforms;
        other.m_ID = 0;
        other.m_uniforms.clear();
    }
    return *this;
}

mylib::ComputeShader::~ComputeShader()
{
    if (m_ID != 0)
    {
        glDeleteProgram(m_ID);
    }
}

// Expects a filepath to the compute shader
void mylib::ComputeShader::assign(std::string filePath)
{
    if (!MIN_OPENGL_VERSION(4,3))
    {
        std::cerr << "MYLIB::ERROR::COMPUTE_SHADER::ASSIGN::OPENGL_VERSION_LESS_THAN_4.3\tCURRENT: " << OPENGL_VERSION_MAJOR << "." << OPENGL_VERSION_MINOR << std::endl;
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

// Specifies the sizes of the work groups in the x, y and z axes
void mylib::ComputeShader::dispatch(uint32_t x, uint32_t y, uint32_t z) const
{
    glDispatchCompute(x, y, z);
}

// Reads a file and returns its content
std::string mylib::ComputeShader::readFile(std::string filePath)
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

void mylib::ComputeShader::checkCompileStatus(GLuint shader, std::string filePath)
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

void mylib::ComputeShader::checkLinkStatus(std::string filePath)
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

GLint mylib::ComputeShader::getUniformLocation(std::string name)
{
    auto it = m_uniforms.find(name);
    if (it != m_uniforms.end())
        return it->second;
    
    GLint location = glGetUniformLocation(m_ID, name.c_str());
#if DEBUG == true
    if (location == -1)
        std::cerr << "MYLIB::ERROR::COMPUTE_SHADER::UNIFORM_NOT_FOUND: " << name << std::endl;
#endif

    m_uniforms.insert({name, location});
    return location;
}

#endif