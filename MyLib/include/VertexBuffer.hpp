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

    void update(const void* data, uint32_t offset, uint32_t size);
private:
    GLuint m_ID;
    uint32_t m_size;
};

} // namespace mylib
