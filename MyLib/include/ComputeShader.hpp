#pragma once

#include "Defines.hpp"
#if MIN_OPENGL_VERSION(4,3)

#include "Shader.hpp"

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

class ComputeShader : public Shader
{
public:
    ComputeShader();
    explicit ComputeShader(const std::string& filePath);
    ComputeShader(ComputeShader&& other) noexcept;
    ComputeShader& operator=(ComputeShader&& other) noexcept;
    ComputeShader(const ComputeShader&) = delete;
    ComputeShader& operator=(const ComputeShader&) = delete;
    virtual ~ComputeShader() = default;

    void assign(const std::string& filePath);
    virtual void recompile() override; // WARNING THIS RESETS UNIFORMS
    inline void dispatch(uint32_t x, uint32_t y, uint32_t z) const { glDispatchCompute(x, y, z); };
    template <typename... Bits>
    void barrier(Bits... bits)
    {
        glMemoryBarrier(bitwiseOr(bits...));
    }

private:
    constexpr inline GLbitfield toGL(MemoryBarrier bit) const { return static_cast<GLbitfield>(bit); }

    template <typename... Bits>
    constexpr GLenum bitwiseOr(Bits... bits)
    {
        static_assert((std::is_same_v<Bits, MemoryBarrier> && ...),
            "All arguments must be mylib::MemoryBarrier");
        return (toGL(bits) | ...);
    }
};

} // namespace mylib

#endif