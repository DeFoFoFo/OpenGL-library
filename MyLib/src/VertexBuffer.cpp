#include "VertexBuffer.hpp"

#include "Renderer.hpp"

mylib::VertexBuffer::VertexBuffer(const void *data, uint32_t size)
{
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

mylib::VertexBuffer::~VertexBuffer()
{
    if (m_ID)
        glDeleteBuffers(1, &m_ID);
}

void mylib::VertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

void mylib::VertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
