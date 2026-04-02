#pragma once

#include "glad/gl.h"

#include "Defines.hpp"

#include <cstdint>

namespace mylib
{

enum class TextureType : GLuint
{
    NONE                     = 0,
    DIFFUSE                  = 1, // Base color of the texture
    SPECULAR                 = 2, // Reflection
};

enum class TextureDimension : GLenum
{
    DIM1                     = GL_TEXTURE_1D,
    DIM2                     = GL_TEXTURE_2D,
//    TEXT3D                     = GL_TEXTURE_3D // Currently not supported
};

enum class WrapDimension : GLenum
{
    WRAP_R                 = GL_TEXTURE_WRAP_R,
    WRAP_X                 = GL_TEXTURE_WRAP_R,
    WRAP_S                 = GL_TEXTURE_WRAP_S,
    WRAP_Y                 = GL_TEXTURE_WRAP_S,
};

enum class WrapParam : GLenum
{
    CLAMP_TO_BORDER        = GL_CLAMP_TO_BORDER,         // Clamps the texture to the border (if specified)
    CLAMP_TO_EDGE          = GL_CLAMP_TO_EDGE,           // Clamps to the edge of the surface
    REPEAT                 = GL_REPEAT,                  // Repeats the texture
    MIRRORED_REPEAT        = GL_MIRRORED_REPEAT,         // Repeats the texture but inverts its direction
#if MIN_OPENGL_VERSION(4,4)
    MIRRORED_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE     // Avoids artifacts when CLAMP_TO_EDGE samples near a border
#endif
};

enum class MinMagFilter : GLenum
{
    MAG                    = GL_TEXTURE_MAG_FILTER,
    MIN                    = GL_TEXTURE_MIN_FILTER,
};

enum class MinMagFilterParam : GLenum
{
    NEAREST                = GL_NEAREST,                 // Closest texel as a pixel (pixelated)
    LINEAR                 = GL_LINEAR,                  // Weighted average of closest texels (blurred)
    NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,  // Closest mipmap and closest texel
    LINEAR_MIPMAP_NEAREST  = GL_LINEAR_MIPMAP_NEAREST,   // Weighted average of mipmaps and closest texel
    NEAREST_MIPMAP_LINEAR  = GL_NEAREST_MIPMAP_LINEAR,   // Closest mipmap and weighted average of texels
    LINEAR_MIPMAP_LINEAR   = GL_LINEAR_MIPMAP_LINEAR     // Weighted average of mipmap levels and texels
};

class Sampler
{
public:
    Sampler(TextureDimension dimension);
    ~Sampler();

    void addWrapParameter(WrapDimension wrapDimension, WrapParam parameter);
    void addMagParameter(MinMagFilter filter, MinMagFilterParam parameter);

    void bind(uint16_t slot = 0) const;
    void unbind(uint16_t slot = 0) const;
private:
    GLuint m_ID;
    GLenum m_dimension = GL_TEXTURE_2D;

    constexpr GLenum toGL(WrapDimension wrapDimension);
    constexpr GLenum toGL(WrapParam param);
    constexpr GLenum toGL(MinMagFilter filter);
    constexpr GLenum toGL(MinMagFilterParam param);
    constexpr GLenum toGL(TextureDimension dimension);
};

class Texture
{
public:
    Texture();
    Texture(TextureDimension dimension, const char* filePath, bool flip = false);
    ~Texture();
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    void loadTexture(TextureDimension dimension, const char* filePath, bool flip = false);
    void loadTexture(TextureDimension dimension, unsigned char* data, size_t size);
    void loadTexture(TextureDimension dimension, int width, int height, unsigned char* data, GLenum format);

    void bind(uint16_t slot = 0) const;
    void unbind() const;

    GLuint ID() const;
    TextureType getTypeName() const;
    void setTypeName(TextureType type);
private:
    GLuint m_ID;
    GLenum m_dimension = GL_TEXTURE_2D;
    TextureType m_typeName; // ex: diffuse, specular, emission

    constexpr GLenum toGL(TextureDimension dimension);
};

} // namespace mylib