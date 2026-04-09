#pragma once

#include "Defines.hpp"
#if MIN_OPENGL_VERSION(4,3)

#include "glad/gl.h"

#include "glm/glm.hpp"

#include <string>
#include <unordered_map>
#include <utility>
#include <type_traits>

namespace mylib
{

enum class MemoryBarrier : GLbitfield
{
    ALL                = GL_ALL_BARRIER_BITS,                  // Use in case of any doubt
    VBO                = GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT,   // Use if VBO is updated and used elsewhere
    EBO                = GL_ELEMENT_ARRAY_BARRIER_BIT,         // Use if EBO is updated and used elsewhere
    IBO                = GL_ELEMENT_ARRAY_BARRIER_BIT,         // Use if IBO is updated and used elsewhere
    UBO                = GL_UNIFORM_BARRIER_BIT,               // Use if UBO is updated and used elsewhere
    TEXT_FETCH         = GL_TEXTURE_FETCH_BARRIER_BIT,         // Use if texture is updated and fetched elsewhere
    IMAGE_ACCESS       = GL_SHADER_IMAGE_ACCESS_BARRIER_BIT,   // Use for image in compute shader
    COMMAND            = GL_COMMAND_BARRIER_BIT,               // Use to issue all previously sent commands
    PBO                = GL_PIXEL_BUFFER_BARRIER_BIT,          // Use if PBO is updated and used elsewhere
    TEXTURE_UPDATE     = GL_TEXTURE_UPDATE_BARRIER_BIT,        // Ensures texture updates (ex: glTexSubImage2D) are visible to subsequent commands
    BUFFER_UPDATE      = GL_BUFFER_UPDATE_BARRIER_BIT,         // Use if buffer is updated by the CPU and used by the GPU
    FBO                = GL_FRAMEBUFFER_BARRIER_BIT,           // Use if FBO is updated and used elsewhere
    TRANSFORM_FEEDBACK = GL_TRANSFORM_FEEDBACK_BARRIER_BIT,    // Use if transform feedback is updated and used elsewhere
    ACBO               = GL_ATOMIC_COUNTER_BARRIER_BIT,        // Use if ACBO is updated and used elsewhere
    SSBO               = GL_SHADER_STORAGE_BARRIER_BIT,        // Use if SSBO is updated and used elsewhere
#if MIN_OPENGL_VERSION(4,4)
    QBO                = GL_QUERY_BUFFER_BARRIER_BIT,          // Use if QBO is updated and used elsewhere
#endif
};

class ComputeShader
{
public:
    ComputeShader();
    explicit ComputeShader(const char* filePath);
    ~ComputeShader();

    void assign(const char* filePath);
    void bind() const;
    void dispatch(uint32_t x, uint32_t y, uint32_t z) const;
    template <typename... Bits>
    void barrier(Bits... bits)
    {
        glMemoryBarrier(bitwiseOr(bits...));
    }
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
    void checkLinkStatus(const char* filePath);
    GLuint getUniformLocation(const std::string name);
    constexpr inline GLbitfield toGL(mylib::MemoryBarrier bit) const {return static_cast<GLbitfield>(bit);}

    template <typename... Bits>
    constexpr GLenum bitwiseOr(Bits... bits)
    {
        static_assert((std::is_same_v<Bits, mylib::MemoryBarrier> && ...),
                      "All arguments must be mylib::MemoryBarrier");
        return (toGL(bits) | ...);
    }
};

} // namespace mylib

#endif