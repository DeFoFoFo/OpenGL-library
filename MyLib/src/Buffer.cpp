#include "Buffer.hpp"

#include <iostream>

mylib::Buffer::Buffer()
{
    glGenBuffers(1, &m_ID);
}

mylib::Buffer::~Buffer()
{
    glDeleteBuffers(1, &m_ID);
}

void mylib::Buffer::fill(const BufferTarget target, GLsizeiptr size, const void *data, GLenum usage)
{
    glBindBuffer(toGL(target), m_ID);
    glBufferData(toGL(target), size, data, usage);
    m_size = size;
}

void mylib::Buffer::update(const BufferTarget target, GLsizeiptr size, GLintptr offset, const void *data)
{
    if (offset + size > m_size)
    {
        std::cerr << "MYLIB::ERROR::BUFFER::UPDATE::SIZE+OFFSET_GREATER_THAN_SIZE" << std::endl;
        return;
    }

    glBindBuffer(toGL(target), m_ID);
    glBufferSubData(toGL(target), offset, size, data);
    m_size = size;
}

void mylib::Buffer::bindAs(const BufferTarget target) const
{
    glBindBuffer(toGL(target), m_ID);
}

void mylib::Buffer::unbindAs(const BufferTarget target) const
{
    glBindBuffer(toGL(target), 0);
}

void mylib::Buffer::bindBase(const BufferTarget target, GLuint index) const
{
    glBindBufferBase(toGL(target), index, m_ID);
}

GLuint mylib::Buffer::ID() const
{
    return m_ID;
}

constexpr GLenum mylib::Buffer::toGL(BufferTarget target) const
{
    return static_cast<GLenum>(target);
}
