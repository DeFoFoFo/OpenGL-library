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

mylib::Buffer::Buffer(Buffer&& other) noexcept
    : m_ID(other.m_ID), m_size(other.m_size)
{
    other.m_ID = 0;
    other.m_size = 0;
}

mylib::Buffer& mylib::Buffer::operator=(Buffer&& other) noexcept
{
    if (this != &other)
    {
        if (m_ID) glDeleteBuffers(1, &m_ID);
        m_ID = other.m_ID;
        m_size = other.m_size;
        other.m_ID = 0;
        other.m_size = 0;
    }
    return *this;
}

void mylib::Buffer::fill(const BufferTarget target, GLsizeiptr size, const void* data, GLenum usage)
{
    glBindBuffer(toGL(target), m_ID);
    glBufferData(toGL(target), size, data, usage);
    m_size = size;
}

void mylib::Buffer::update(const BufferTarget target, GLsizeiptr size, GLintptr offset, const void* data)
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