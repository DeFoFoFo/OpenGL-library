#pragma once

#include "Buffer.hpp"
#include "VertexBufferLayout.hpp"

namespace mylib
{

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();
    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(VertexArray&& other) noexcept;
    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;

    void addBuffer(const Buffer& vb, const VertexBufferLayout& layout);
    void bind() const;
    void unbind() const;

    constexpr inline GLuint ID() const {return m_ID;}
private:
    GLuint m_ID;
};

} // namespace mylib