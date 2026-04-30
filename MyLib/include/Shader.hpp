#pragma once

#include "glad/gl.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <unordered_map>
#include <vector>
#include <string>

namespace mylib
{

class Shader
{
public:
    Shader();
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    virtual ~Shader();

    inline void bind() const { glUseProgram(m_ID); }
    inline GLuint ID() const { return m_ID; }
    virtual void recompile() = 0;

    inline void setUniform(const int uniform, const std::string& name) { glUniform1i(getUniformLocation(name), uniform); }
    inline void setUniform(const uint32_t uniform, const std::string& name) { glUniform1ui(getUniformLocation(name), uniform); }
    inline void setUniform(const float uniform, const std::string& name) { glUniform1f(getUniformLocation(name), uniform); }
    inline void setUniform(const glm::vec2 uniform, const std::string& name) { glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(uniform)); }
    inline void setUniform(const glm::vec3 uniform, const std::string& name) { glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(uniform)); }
    inline void setUniform(const glm::vec4 uniform, const std::string& name) { glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(uniform)); }
    inline void setUniform(const glm::mat3 uniform, const std::string& name) { glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(uniform)); }
    inline void setUniform(const glm::mat4 uniform, const std::string& name) { glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(uniform)); }
protected:
    GLuint m_ID;
    std::unordered_map<std::string, GLuint> m_uniforms;
    std::vector<std::string> m_fileNames;

    std::string readFile(const std::string& filePath);
    void checkCompileStatus(GLuint shader, const std::string& filePath);
    void checkLinkStatus();
    GLint getUniformLocation(const std::string& name);
};

} // namespace mylib