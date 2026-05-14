#include "Texture.hpp"

#include "GLFW/glfw3.h"

#include "stb_image.h"

#include <iostream>

mylib::Texture::Texture()
{
    glGenTextures(1, &m_ID);
}

mylib::Texture::Texture(TextureDimension dimension, const char* filePath, bool flip)
{
    glGenTextures(1, &m_ID);
    loadTexture(dimension, filePath, flip);
}

mylib::Texture::~Texture()
{
    if (m_ID)
        glDeleteTextures(1, &m_ID);
}

mylib::Texture::Texture(Texture&& other) noexcept
    : m_ID(other.m_ID), m_dimension(other.m_dimension), m_typeName(other.m_typeName)
{
    other.m_ID = 0;
}

mylib::Texture& mylib::Texture::operator=(Texture&& other) noexcept
{
    if (this != &other) {
        if (m_ID) glDeleteTextures(1, &m_ID);
        m_ID = other.m_ID;
        other.m_ID = 0;
        m_dimension = other.m_dimension;
        m_typeName = other.m_typeName;
    }
    return *this;
}

static void addDefault1DTextureFilters()
{
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

static void addDefault2DTextureFilters()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void mylib::Texture::loadTexture(TextureDimension dimension, const char* filePath, bool flip)
{
#ifdef MYLIB_DEBUG
    double startTime = glfwGetTime();
#endif

    m_dimension = static_cast<GLenum>(dimension);

    bind();

    stbi_set_flip_vertically_on_load(flip);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 4);

    stbi_set_flip_vertically_on_load(false);

    if (data)
    {
        GLint prevAlignment;
        glGetIntegerv(GL_UNPACK_ALIGNMENT, &prevAlignment);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        switch (m_dimension)
        {
        case GL_TEXTURE_1D:
        {
            addDefault1DTextureFilters();
            glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA8, width, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            break;
        }
        case GL_TEXTURE_2D:
        {
            addDefault2DTextureFilters();
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            break;
        }
        }
        glGenerateMipmap(m_dimension);

        glPixelStorei(GL_UNPACK_ALIGNMENT, prevAlignment);
    }
    else
        std::cerr << "MYLIB::ERROR::TEXTURE::FAILED_TO_LOAD_TEXTURE\tPATH:" << filePath << std::endl;

    stbi_image_free(data);

#ifdef MYLIB_DEBUG
    double timeOfLoading = glfwGetTime() - startTime;
    std::cout << "MYLIB::TEXTURE::LOADED_IN " << timeOfLoading * 1000 << "ms\n"
        << "\tPATH: " << filePath << std::endl;
#endif
}

void mylib::Texture::loadTexture(TextureDimension dimension, unsigned char* data, size_t size)
{
#ifdef MYLIB_DEBUG
    double startTime = glfwGetTime();
#endif

    m_dimension = static_cast<GLenum>(dimension);

    bind();

    int width, height, nrChannels;
    unsigned char* decoded = stbi_load_from_memory(data, size, &width, &height, &nrChannels, 4);

    if (decoded)
    {
        GLint prevAlignment;
        glGetIntegerv(GL_UNPACK_ALIGNMENT, &prevAlignment);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        switch (m_dimension)
        {
        case GL_TEXTURE_1D:
        {
            addDefault1DTextureFilters();
            glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA8, width, 0, GL_RGBA, GL_UNSIGNED_BYTE, decoded);
            break;
        }
        case GL_TEXTURE_2D:
        {
            addDefault2DTextureFilters();
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, decoded);
            break;
        }
        }
        glGenerateMipmap(m_dimension);

        glPixelStorei(GL_UNPACK_ALIGNMENT, prevAlignment);
    }
    else
        std::cerr << "MYLIB::ERROR::TEXTURE::FAILED_TO_LOAD_TEXTURE_FROM_MEMORY" << std::endl;

    stbi_image_free(decoded);

#ifdef MYLIB_DEBUG
    double loadingTime = glfwGetTime() - startTime;
    std::cout << "MYLIB::TEXTURE::LOADED_IN " << loadingTime * 1000 << "ms" << std::endl;
#endif
}

void mylib::Texture::loadTexture(TextureDimension dimension, int width, int height, unsigned char* data, GLenum format)
{
#ifdef MYLIB_DEBUG
    double startTime = glfwGetTime();
#endif

    if (!data)
    {
        std::cerr << "MYLIB::ERROR::TEXTURE::LOADED_NULL_DATA" << std::endl;
    }
    m_dimension = toGL(dimension);

    bind();

    GLint prevAlignment;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &prevAlignment);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    switch (m_dimension)
    {
    case GL_TEXTURE_1D:
    {
        addDefault1DTextureFilters();
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA8, width, 0, format, GL_UNSIGNED_BYTE, data);
        break;
    }
    case GL_TEXTURE_2D:
    {
        addDefault2DTextureFilters();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        break;
    }
    }
    glGenerateMipmap(m_dimension);

    glPixelStorei(GL_UNPACK_ALIGNMENT, prevAlignment);

#ifdef MYLIB_DEBUG
    double timeOfLoading = glfwGetTime() - startTime;
    std::cout << "MYLIB::TEXTURE::LOADED_IN " << timeOfLoading * 1000 << "ms" << std::endl;
#endif
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

mylib::Sampler::Sampler(TextureDimension dimension)
    : m_dimension{ toGL(dimension) }
{
    glGenSamplers(1, &m_ID);
}

mylib::Sampler::~Sampler()
{
    if (m_ID)
        glDeleteSamplers(1, &m_ID);
}

void mylib::Sampler::addWrapParameter(Wrap wrapDimension, WrapParam parameter)
{
    glSamplerParameteri(m_ID, toGL(wrapDimension), toGL(parameter));
}

void mylib::Sampler::addMagParameter(MinMagFilter filter, MinMagFilterParam parameter)
{
    glSamplerParameteri(m_ID, toGL(filter), toGL(parameter));
}

void mylib::Sampler::bind(uint16_t slot) const
{
    glBindSampler(slot, m_ID);
}

void mylib::Sampler::unbind(uint16_t slot) const
{
    glBindSampler(slot, 0);
}