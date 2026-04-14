#pragma once

#include "glad/gl.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <string>

namespace mylib
{

class Shader
{
public:
    Shader();
    explicit Shader(std::string vsFilePath, std::string fsFilePath);
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;
    Shader(const Shader&) = delete;
    Shader operator=(const Shader&) = delete;
    ~Shader();

    void assign(std::string vsFilePath, std::string fsFilePath);
    inline void bind() const {glUseProgram(m_ID);}
    constexpr inline GLuint ID() const {return m_ID;}
    void recompile();

    inline void setUniform(const int uniform, std::string name) {glUniform1i(getUniformLocation(name), uniform);}
    inline void setUniform(const uint32_t uniform, std::string name) {glUniform1ui(getUniformLocation(name), uniform);}
    inline void setUniform(const float uniform, std::string name) {glUniform1f(getUniformLocation(name), uniform);}
    inline void setUniform(const glm::vec2 uniform, std::string name) {glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(uniform));}
    inline void setUniform(const glm::vec3 uniform, std::string name) {glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(uniform));}
    inline void setUniform(const glm::vec4 uniform, std::string name) {glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(uniform));}
    inline void setUniform(const glm::mat3 uniform, std::string name) {glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(uniform));}
    inline void setUniform(const glm::mat4 uniform, std::string name) {glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(uniform));}
private:
    GLuint m_ID;
    std::unordered_map<std::string, GLuint> m_uniforms;
    
    std::string readFile(std::string filePath);
    void checkCompileStatus(GLuint shader, std::string filePath);
    void checkLinkStatus(std::string vsFilePath, std::string fsFilePath);
    GLint getUniformLocation(const std::string name);
};

} // namespace mylib