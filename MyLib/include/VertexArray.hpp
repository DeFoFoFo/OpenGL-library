#pragma once

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

namespace mylib
{

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
    void bind() const;
    void unbind() const;
private:
    GLuint m_ID;
};

} // namespace mylib