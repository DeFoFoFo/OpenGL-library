#include "VertexBuffer.hpp"

#include "Renderer.hpp"

#include <iostream>

mylib::VertexBuffer::VertexBuffer(const void *data, uint32_t size)
    : m_size{size}
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

// This function changes the data contained in the buffer, it changes a subset of the buffer or the entirety but doesn't allocate new memory
void mylib::VertexBuffer::update(const void *data, uint32_t offset, uint32_t size)
{
    if (size + offset > m_size)
    {
        std::cerr << "MYLIB::ERROR::VERTEX_BUFFER::UPDATE::SIZE+OFFSET_GREATER_THAN_CURRENT_SIZE" << std::endl;
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}
