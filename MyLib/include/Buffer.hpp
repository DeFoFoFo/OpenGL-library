#pragma once

#include "glad/gl.h"

#include "Defines.hpp"

namespace mylib
{

enum class BufferTarget : GLenum
{
    VBO         = GL_ARRAY_BUFFER,
    IBO         = GL_ELEMENT_ARRAY_BUFFER, // alias to EBO
    EBO         = GL_ELEMENT_ARRAY_BUFFER, // alias to IBO
    TEXTURE     = GL_TEXTURE_BUFFER,
    QUERY       = GL_QUERY_BUFFER,
    UBO         = GL_UNIFORM_BUFFER,
    COPY        = GL_COPY_READ_BUFFER,
    WRITE       = GL_COPY_WRITE_BUFFER,
#if MIN_OPENGL_VERSION(4,2)
    ACB         = GL_ATOMIC_COUNTER_BUFFER,
#endif
#if MIN_OPENGL_VERSION(4,3)
    SSBO        = GL_SHADER_STORAGE_BUFFER,
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

    void bindAs(const BufferTarget target) const;
    void unbindAs(const BufferTarget target) const;
    void bindBase(const BufferTarget target, GLuint index) const;

    GLuint ID() const;
private:
    GLuint m_ID = 0;
    GLuint m_size = 0;

    constexpr GLenum toGL(BufferTarget target) const;
};

} // namespace mylib