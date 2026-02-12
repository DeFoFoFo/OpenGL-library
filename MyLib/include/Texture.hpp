#pragma once

#include "glad/gl.h"

#include <cstdint>

namespace mylib
{

class Texture
{
public:
    Texture();
    Texture(const char* filePath);
    ~Texture();

    void loadTexture(const char* filePath);

    void bind(uint16_t slot = 0);
    void unbind();
private:
    GLuint m_ID;
};

} // namespace mylib