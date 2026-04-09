#pragma once

#include "glad/gl.h"

#include "glm/glm.hpp"

#include <string>

namespace mylib
{

class Shader
{
public:
    Shader();
    explicit Shader(const char* vsFilePath, const char* fsFilePath);
    ~Shader();

    void assign(const char* vsFilePath, const char* fsFilePath);
    void bind() const;
    constexpr inline GLuint ID() const {return m_ID;}

    void setUniform(const int uniform, const std::string name);
    void setUniform(const uint32_t uniform, const std::string name);
    void setUniform(const float uniform, const std::string name);
    void setUniform(const glm::vec2 uniform, const std::string name);
    void setUniform(const glm::vec3 uniform, const std::string name);
    void setUniform(const glm::vec4 uniform, const std::string name);
    void setUniform(const glm::mat3 uniform, const std::string name);
    void setUniform(const glm::mat4 uniform, const std::string name);
private:
    GLuint m_ID;
    std::unordered_map<std::string, GLuint> m_uniforms;
    
    std::string readFile(const char* filePath);
    void checkCompileStatus(GLuint shader, const char* filePath);
    void checkLinkStatus(const char* vsFilePath, const char* fsFilePath);
    GLuint getUniformLocation(const std::string name);
};

} // namespace mylib