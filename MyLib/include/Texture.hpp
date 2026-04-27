#pragma once

#include "glad/gl.h"

#include "Defines.hpp"

#include <cstdint>
#include <cstddef>

namespace mylib
{

enum class TextureType : GLuint
{
    NONE     = 0,
    DIFFUSE  = 1,   // Base color of the texture
    SPECULAR = 2,   // Reflection
};

enum class TextureDimension : GLenum
{
    DIM1 = GL_TEXTURE_1D,
    DIM2 = GL_TEXTURE_2D,
    DIM3 = GL_TEXTURE_3D,   // Currently not supported
};

enum class Wrap : GLenum
{
    WRAP_R = GL_TEXTURE_WRAP_R,   // Wrap the texture in the x dimension
    WRAP_X = GL_TEXTURE_WRAP_R,   // Wrap the texture in the x dimension
    WRAP_S = GL_TEXTURE_WRAP_S,   // Wrap the texture in the y dimension
    WRAP_Y = GL_TEXTURE_WRAP_S,   // Wrap the texture in the y dimension
    WRAP_T = GL_TEXTURE_WRAP_T,   // Wrap the texture in the z dimension
    WRAP_Z = GL_TEXTURE_WRAP_T,   // Wrap the texture in the z dimension
};

enum class WrapParam : GLenum
{
    CLAMP_TO_BORDER        = GL_CLAMP_TO_BORDER,      // Clamps the texture to the border (if specified)
    CLAMP_TO_EDGE          = GL_CLAMP_TO_EDGE,        // Clamps to the edge of the surface
    REPEAT                 = GL_REPEAT,               // Repeats the texture
    MIRRORED_REPEAT        = GL_MIRRORED_REPEAT,      // Repeats the texture but inverts its direction
#if MIN_OPENGL_VERSION(4,4)
    MIRRORED_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE  // Avoids artifacts when CLAMP_TO_EDGE samples near a border
#endif
};

enum class MinMagFilter : GLenum
{
    MAG = GL_TEXTURE_MAG_FILTER,   // Magnifying filter
    MIN = GL_TEXTURE_MIN_FILTER,   // Minifying filter
};

enum class MinMagFilterParam : GLenum
{
    NEAREST                = GL_NEAREST,                  // Closest texel as a pixel (pixelated)
    LINEAR                 = GL_LINEAR,                   // Weighted average of closest texels (blurred)
    NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,   // Closest mipmap and closest texel
    LINEAR_MIPMAP_NEAREST  = GL_LINEAR_MIPMAP_NEAREST,    // Weighted average of mipmaps and closest texel
    NEAREST_MIPMAP_LINEAR  = GL_NEAREST_MIPMAP_LINEAR,    // Closest mipmap and weighted average of texels
    LINEAR_MIPMAP_LINEAR   = GL_LINEAR_MIPMAP_LINEAR      // Weighted average of mipmap levels and texels
};

class Sampler
{
public:
    Sampler(TextureDimension dimension);
    ~Sampler();

    void addWrapParameter(Wrap wrapDimension, WrapParam parameter);
    void addMagParameter(MinMagFilter filter, MinMagFilterParam parameter);

    void bind(uint16_t slot = 0) const;
    void unbind(uint16_t slot = 0) const;
private:
    GLuint m_ID;
    GLenum m_dimension = GL_TEXTURE_2D;

    constexpr inline GLenum toGL(Wrap param) { return static_cast<GLenum>(param); }
    constexpr inline GLenum toGL(WrapParam param) { return static_cast<GLenum>(param); }
    constexpr inline GLenum toGL(MinMagFilter param) { return static_cast<GLenum>(param); }
    constexpr inline GLenum toGL(MinMagFilterParam param) { return static_cast<GLenum>(param); }
    constexpr inline GLenum toGL(TextureDimension param) { return static_cast<GLenum>(param); }
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

    constexpr inline GLuint ID() const { return m_ID; }
    constexpr inline TextureType getTypeName() const { return m_typeName; }
    inline void setTypeName(TextureType type) { m_typeName = type; }
private:
    GLuint m_ID;
    GLenum m_dimension = GL_TEXTURE_2D;
    TextureType m_typeName; // ex: diffuse, specular, emission

    constexpr inline GLenum toGL(TextureDimension param) { return static_cast<GLenum>(param); }
};

} // namespace mylib
