#pragma once

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
    GLuint getID();
private:
    GLuint m_ID;

    std::string readFile(const char* filePath);
    void checkCompileStatus(GLuint shader, const char* filePath);
    void checkLinkStatus(const char* filePath);
};

}