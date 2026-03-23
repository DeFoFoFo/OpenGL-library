#pragma once

#include "Defines.hpp"
#if MIN_OPENGL_VERSION(4,3)

#include "glad/gl.h"

#include <cstdint>

namespace mylib
{

class ShaderStorageBuffer
{
public:
    ShaderStorageBuffer(const void* data, uint32_t size, GLenum usage = GL_STATIC_DRAW);
    ~ShaderStorageBuffer();

    void bind(uint32_t base = 0) const;
    void unbind() const;
private:
    GLuint m_ID;
    uint32_t m_size;
};

    
} // namespace mylib

#endif