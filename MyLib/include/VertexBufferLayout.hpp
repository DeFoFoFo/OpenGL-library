#pragma once

#include "glad/gl.h"

#include <vector>
#include <cstdint>
#include <iostream>

namespace mylib
{

struct VertexBufferElement
{
    GLuint type;
    uint32_t count;
    GLuint normalized;

    static uint32_t getSizeOfType(GLuint type)
    {
        switch (type)
        {
        case GL_FLOAT: return sizeof(GLfloat);
        case GL_UNSIGNED_INT: return sizeof(GLuint);
        case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
        default:
        {
            std::cerr << "MYLIB::ERROR::VERTEXBUFFERLAYOUT::UNSUPPORTED_TYPE" << std::endl;
            return 0;
        }
        }
    }
};

class VertexBufferLayout
{
public:
    VertexBufferLayout() : m_stride{0}
    {}

    void push(uint32_t count, GLuint elemType)
    {
        switch (elemType)
        {
        case GL_FLOAT:
            m_elements.push_back({GL_FLOAT, count, GL_FALSE});
            m_stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
            break;
        case GL_UNSIGNED_INT:
            m_elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
            m_stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
            break;
        case GL_UNSIGNED_BYTE:
            m_elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
            m_stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
            break;
        default:
            std::cerr << "MYLIB::ERROR::VERTEXBUFFERLAYOUT::PUSH::UNSUPPORTED_TYPE" << std::endl;
        }
    }

    constexpr inline const std::vector<VertexBufferElement>& getElements() const {return m_elements;}
    constexpr inline uint32_t getStride() const {return m_stride;}

private:
    std::vector<VertexBufferElement> m_elements;
    uint32_t m_stride;
};

} // namespace mylib