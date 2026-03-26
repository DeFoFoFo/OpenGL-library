#pragma once

#include "glad/gl.h"

#include "Defines.hpp"

#include <cstdint>

namespace mylib
{

enum class TextureType : GLuint
{
    TEXT1D                     = GL_TEXTURE_1D,
    TEXT2D                     = GL_TEXTURE_2D,
//    TEXT3D                     = GL_TEXTURE_3D // Currently not supported
};

enum class WrapParam : GLuint
{
    CLAMP_TO_BORDER        = GL_CLAMP_TO_BORDER,         // Clamps the texture to the border (if specified)
    CLAMP_TO_EDGE          = GL_CLAMP_TO_EDGE,           // Clamps to the edge of the surface
    REPEAT                 = GL_REPEAT,                  // Repeats the texture
    MIRRORED_REPEAT        = GL_MIRRORED_REPEAT,         // Repeats the texture but inverts its direction
#if MIN_OPENGL_VERSION(4,4)
    MIRRORED_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE     // Avoids artifacts when CLAMP_TO_EDGE samples near a border
#endif
};
enum class MinMagFilterParam : GLuint
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
    Sampler(TextureType type);
    ~Sampler();

    void addWrapParameter(GLenum wrapDimension, WrapParam parameter);
    void addMagParameter(GLenum filter, MinMagFilterParam parameter);

    void bind(uint16_t slot = 0) const;
    void unbind(uint16_t slot = 0) const;
private:
    GLuint m_ID;
    GLenum m_type;
};

class Texture
{
public:
    Texture(TextureType type);
    Texture(TextureType type, const char* filePath);
    ~Texture();

    void loadTexture(const char* filePath);

    void bind(uint16_t slot = 0) const;
    void unbind() const;
private:
    GLuint m_ID;
    GLenum m_type;
};

} // namespace mylib