#include "ShaderStorageBuffer.hpp"

#if MIN_OPENGL_VERSION(4,3)

mylib::ShaderStorageBuffer::ShaderStorageBuffer(const void *data, uint32_t size, GLenum usage)
    : m_size{size}
{
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, usage);
}

mylib::ShaderStorageBuffer::~ShaderStorageBuffer()
{
    if (m_ID)
        glDeleteBuffers(1, &m_ID);
}

void mylib::ShaderStorageBuffer::bind(uint32_t base) const
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ID);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, base, m_ID);
}

void mylib::ShaderStorageBuffer::unbind() const
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

#endif