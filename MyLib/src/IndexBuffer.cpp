#include "IndexBuffer.hpp"

mylib::IndexBuffer::IndexBuffer(const GLuint *data, GLuint count)
    : m_count{count}
{
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(GLuint), data, GL_STATIC_DRAW);
}

mylib::IndexBuffer::~IndexBuffer()
{
    if (m_ID)
        glDeleteBuffers(1, &m_ID);
}

void mylib::IndexBuffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

void mylib::IndexBuffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLuint mylib::IndexBuffer::getCount() const
{
    return m_count;
}
