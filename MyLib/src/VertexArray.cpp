#include "VertexArray.hpp"

#include "glad/gl.h"

mylib::VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_ID);
    bind();
}

mylib::VertexArray::~VertexArray()
{
    if (m_ID)
        glDeleteVertexArrays(1, &m_ID);
}

mylib::VertexArray::VertexArray(VertexArray &&other) noexcept
    : m_ID{other.m_ID}
{
    other.m_ID = 0;
}

mylib::VertexArray &mylib::VertexArray::operator=(VertexArray &&other) noexcept
{
    if (this != &other) {
        if (m_ID) glDeleteVertexArrays(1, &m_ID);
        m_ID = other.m_ID;
        other.m_ID = 0;
    }
    return *this;
}

void mylib::VertexArray::addBuffer(const Buffer &vb, const VertexBufferLayout &layout)
{
    bind();
    vb.bindAs(mylib::BufferTarget::VBO);
    const auto& elements = layout.getElements();
    uint64_t offset{};
    for (size_t i{}; i < elements.size(); ++i)
    {
        const auto& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset);
        offset += element.count * VertexBufferElement::getSizeOfType(element.type);
    }
    
}

void mylib::VertexArray::bind() const
{
    glBindVertexArray(m_ID);
}

void mylib::VertexArray::unbind() const
{
    glBindVertexArray(0);
}
