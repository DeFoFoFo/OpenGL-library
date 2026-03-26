#include "Texture.hpp"

#include "stb_image.h"

#include <iostream>

mylib::Texture::Texture(TextureType type)
    : m_type{static_cast<GLenum>(type)}
{
    glGenTextures(1, &m_ID);
}

mylib::Texture::Texture(TextureType type, const char *filePath)
    : m_type{static_cast<GLenum>(type)}
{
    glGenTextures(1, &m_ID);
    loadTexture(filePath);
}

mylib::Texture::~Texture()
{
    if (m_ID)
        glDeleteTextures(1, &m_ID);
}

void mylib::Texture::loadTexture(const char *filePath)
{
    bind();

    stbi_set_flip_vertically_on_load(true);
    
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);

    if (data)
    {
        GLenum format{GL_RGBA};
        switch (nrChannels)
        {
            case 1:
            {
                format = GL_RED;
                break;
            }
            case 2:
            {
                format = GL_RG;
                break;
            }
            case 3:
            {
                format = GL_RGB;
                break;
            }
            case 4:
            {
                format = GL_RGBA;
                break;
            }
            default:
            {
                std::cerr << "MYLIB::ERROR::TEXTURE::UNSUPPORTED_FORMAT\tPATH: " << filePath << "\tCHANNELS: " << nrChannels << std::endl; 
            }
        }

        switch (m_type)
        {
            case GL_TEXTURE_1D:
            {
                glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, width, 0, format, GL_UNSIGNED_BYTE, data);
                break;
            }
            case GL_TEXTURE_2D:
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                break;
            }
        }
        glGenerateMipmap(m_type);
    }
    else
        std::cerr << "MYLIB::ERROR::TEXTURE::FAILED_TO_LOAD_TEXTURE\tPATH:" << filePath << std::endl;

    stbi_image_free(data);
}

void mylib::Texture::bind(uint16_t slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(m_type, m_ID);
}

void mylib::Texture::unbind() const
{
    glBindTexture(m_type, 0);
}

mylib::Sampler::Sampler(TextureType type)
    : m_type{static_cast<GLenum>(type)}
{
    glGenSamplers(1, &m_ID);
}

mylib::Sampler::~Sampler()
{
    if (m_ID)
        glDeleteSamplers(1, &m_ID);
}

void mylib::Sampler::addWrapParameter(GLenum wrapDimension, WrapParam parameter)
{
    glSamplerParameteri(m_type, wrapDimension, static_cast<GLuint>(parameter));
}

void mylib::Sampler::addMagParameter(GLenum filter, MinMagFilterParam parameter)
{
    glSamplerParameteri(m_type, filter, static_cast<GLuint>(parameter));
}

void mylib::Sampler::bind(uint16_t slot) const
{
    glBindSampler(GL_TEXTURE0 + slot, m_ID);
}

void mylib::Sampler::unbind(uint16_t slot) const
{
    glBindSampler(GL_TEXTURE0 + slot, 0);
}
