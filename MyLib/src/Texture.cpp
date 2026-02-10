#include "Texture.hpp"

#include "stb_image.h"

#include <iostream>

mylib::Texture::Texture()
    : m_ID{0}
{
    glGenTextures(1, &m_ID);
    bind();
}

mylib::Texture::Texture(const char *filePath)
    : m_ID{0}
{
    glGenTextures(1, &m_ID);
    bind();
    loadTexture(filePath);
}

mylib::Texture::~Texture()
{
    if (m_ID)
        glDeleteTextures(1, &m_ID);
}

void mylib::Texture::loadTexture(const char *filePath)
{
    stbi_set_flip_vertically_on_load(true);
    
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);

    if (data)
    {
        GLenum format{GL_RGBA8};
        switch (nrChannels)
        {
            case 1:
            {
                format = GL_R8;
                break;
            }
            case 2:
            {
                format = GL_RG8;
                break;
            }
            case 3:
            {
                format = GL_RGB8;
                break;
            }
            case 4:
            {
                format = GL_RGBA8;
                break;
            }
            default:
            {
                std::cerr << "MYLIB::ERROR::TEXTURE::UNSUPPORTED_FORMAT\tPATH: " << filePath << "\tCHANNELS: " << nrChannels << std::endl; 
            }
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cerr << "MYLIB::ERROR::TEXTURE::FAILED_TO_LOAD_TEXTURE\tPATH:" << filePath << std::endl;

    stbi_image_free(data);
}

void mylib::Texture::bind(uint16_t slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_ID);
}

void mylib::Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
