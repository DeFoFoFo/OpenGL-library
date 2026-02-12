#pragma once

#include "glad/gl.h"

#include <cstdint>

namespace mylib
{
    
class VertexBuffer
{
public:
    VertexBuffer(const void* data, uint32_t size);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;
private:
    GLuint m_ID;
};

} // namespace mylib
