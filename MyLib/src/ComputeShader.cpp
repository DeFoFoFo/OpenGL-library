#include "ComputeShader.hpp"

#include "Defines.hpp"
#if MIN_OPENGL_VERSION(4,3)

#include <iostream>
#include <fstream>
#include <sstream>

#include "glm/gtc/type_ptr.hpp"

mylib::ComputeShader::ComputeShader()
{}

// Expects a filepath to the compute shader
mylib::ComputeShader::ComputeShader(const char* filePath)
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

void mylib::ComputeShader::bind() const
{
    glUseProgram(m_ID);
}

// Specifies the sizes of the work groups in the x, y and z axes
void mylib::ComputeShader::dispatch(uint32_t x, uint32_t y, uint32_t z) const
{
    glDispatchCompute(x, y, z);
}

void mylib::ComputeShader::setUniform(const int uniform, const std::string name)
{
    bind();
    glUniform1i(getUniformLocation(name), uniform);
}

void mylib::ComputeShader::setUniform(const uint32_t uniform, const std::string name)
{
    bind();
    glUniform1ui(getUniformLocation(name), uniform);
}

void mylib::ComputeShader::setUniform(const float uniform, const std::string name)
{
    bind();
    glUniform1f(getUniformLocation(name), uniform);
}

void mylib::ComputeShader::setUniform(const glm::vec2 uniform, const std::string name)
{
    bind();
    glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(uniform));
}

void mylib::ComputeShader::setUniform(const glm::vec3 uniform, const std::string name)
{
    bind();
    glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(uniform));
}

void mylib::ComputeShader::setUniform(const glm::vec4 uniform, const std::string name)
{
    bind();
    glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(uniform));
}

void mylib::ComputeShader::setUniform(const glm::mat3 uniform, const std::string name)
{
    bind();
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(uniform));
}

void mylib::ComputeShader::setUniform(const glm::mat4 uniform, const std::string name)
{
    bind();
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(uniform));
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

GLuint mylib::ComputeShader::getUniformLocation(const std::string name)
{
    auto it = m_uniforms.find(name);
    if (it != m_uniforms.end())
        return it->second;
    
    GLuint location = glGetUniformLocation(m_ID, name.data());
    m_uniforms.insert({name, location});
    return location;
}

#endif