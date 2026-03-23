#pragma once

#include "Defines.hpp"
#if MIN_OPENGL_VERSION(4,3)

#include "glad/gl.h"

#include <string>

namespace mylib
{

class ComputeShader
{
public:
    ComputeShader();
    explicit ComputeShader(const char* filePath);
    ~ComputeShader();

    void assign(const char* filePath);
    void bind() const;
    void dispatch(uint32_t x, uint32_t y, uint32_t z) const;
    GLuint getID();
private:
    GLuint m_ID;

    std::string readFile(const char* filePath);
    void checkCompileStatus(GLuint shader, const char* filePath);
    void checkLinkStatus(const char* filePath);
};

} // namespace mylib

#endif