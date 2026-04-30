#pragma once

#include "glad/gl.h"

#include "Defines.hpp"

namespace mylib
{

enum class BufferTarget : GLenum
{
    VBO   = GL_ARRAY_BUFFER,           // Array buffer
    IBO   = GL_ELEMENT_ARRAY_BUFFER,   // Element array buffer (alias to EBO)
    EBO   = GL_ELEMENT_ARRAY_BUFFER,   // Element array buffer (alias to IBO)
    TBO   = GL_TEXTURE_BUFFER,         // Texture buffer
    QBO   = GL_QUERY_BUFFER,           // Query buffer
    UBO   = GL_UNIFORM_BUFFER,         // Uniform buffer
    READ  = GL_COPY_READ_BUFFER,       // Copy read buffer
    WRITE = GL_COPY_WRITE_BUFFER,      // Copy write buffer
#if MIN_OPENGL_VERSION(4,2)
    ACBO  = GL_ATOMIC_COUNTER_BUFFER,  // Atomic counter buffer
#endif
#if MIN_OPENGL_VERSION(4,3)
    SSBO  = GL_SHADER_STORAGE_BUFFER,  // Shader storage buffer
#endif
};

class Buffer
{
public:
    Buffer();
    ~Buffer();
    Buffer(Buffer&& other) noexcept;
    Buffer& operator=(Buffer&& other) noexcept;
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;


    void fill(const BufferTarget target, GLsizeiptr size, const void* data, GLenum usage);
    void update(const BufferTarget target, GLsizeiptr size, GLintptr offset, const void* data);

    inline void bindAs(const BufferTarget target) const { glBindBuffer(toGL(target), m_ID); }
    inline void unbindAs(const BufferTarget target) const { glBindBuffer(toGL(target), 0); }
    inline void bindBase(const BufferTarget target, GLuint index) const { glBindBufferBase(toGL(target), index, m_ID); }

    constexpr inline GLuint ID() const { return m_ID; }
private:
    GLuint m_ID = 0;
    GLuint m_size = 0;

    constexpr inline GLenum toGL(BufferTarget target) const { return static_cast<GLenum>(target); }
};

} // namespace mylib