#include "Texture.hpp"

#include "stb_image.h"

#include <iostream>

mylib::Texture::Texture()
{
    glGenTextures(1, &m_ID);
}

mylib::Texture::Texture(TextureDimension dimension, const char *filePath, bool flip)
{
    glGenTextures(1, &m_ID);
    loadTexture(dimension, filePath, flip);
}

mylib::Texture::~Texture()
{
    if (m_ID)
        glDeleteTextures(1, &m_ID);
}

void mylib::Texture::loadTexture(TextureDimension dimension, const char *filePath, bool flip)
{
    m_dimension = static_cast<GLenum>(dimension);

    bind();

    stbi_set_flip_vertically_on_load(flip);
    
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

        switch (m_dimension)
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
        glGenerateMipmap(m_dimension);
    }
    else
        std::cerr << "MYLIB::ERROR::TEXTURE::FAILED_TO_LOAD_TEXTURE\tPATH:" << filePath << std::endl;

    stbi_image_free(data);
}

void mylib::Texture::bind(uint16_t slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(m_dimension, m_ID);
}

void mylib::Texture::unbind() const
{
    glBindTexture(m_dimension, 0);
}

GLuint mylib::Texture::ID() const
{
    return m_ID;
}

mylib::TextureType mylib::Texture::getTypeName() const
{
    return m_typeName;
}

void mylib::Texture::setTypeName(mylib::TextureType type)
{
    m_typeName = type;
}

mylib::Sampler::Sampler(TextureDimension dimension)
    : m_dimension{static_cast<GLenum>(dimension)}
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
    glSamplerParameteri(m_dimension, wrapDimension, static_cast<GLuint>(parameter));
}

void mylib::Sampler::addMagParameter(GLenum filter, MinMagFilterParam parameter)
{
    glSamplerParameteri(m_dimension, filter, static_cast<GLuint>(parameter));
}

void mylib::Sampler::bind(uint16_t slot) const
{
    glBindSampler(GL_TEXTURE0 + slot, m_ID);
}

void mylib::Sampler::unbind(uint16_t slot) const
{
    glBindSampler(GL_TEXTURE0 + slot, 0);
}
