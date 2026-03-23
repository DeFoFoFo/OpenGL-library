#pragma once

#include "glad/gl.h"

#include <cstdint>

namespace mylib
{
    
class IndexBuffer
{
public:
    IndexBuffer(const GLuint* data, GLuint count);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;

    GLuint getCount() const;
private:
    GLuint m_ID;
    GLuint m_count;
};

} // namespace mylib
