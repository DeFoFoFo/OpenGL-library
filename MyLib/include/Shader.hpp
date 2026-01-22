#pragma once

#include "glad/gl.h"

#include <string>

namespace mylib
{

class Shader
{
public:
    Shader();
    explicit Shader(const char* vsFilePath, const char* fsFilePath);
    ~Shader();

    void assign(const char* vsFilePath, const char* fsFilePath);
    GLuint ID();
private:
    GLuint m_ID;
    
    std::string readFile(const char* filePath);
    void checkCompileStatus(GLuint shader, const char* filePath);
    void checkLinkStatus(const char* vsFilePath, const char* fsFilePath);
};

}