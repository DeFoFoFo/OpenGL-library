#pragma once

#include "Shader.hpp"

namespace mylib
{

class ShaderProgram : public Shader
{
public:
    ShaderProgram();
    explicit ShaderProgram(const std::string& vsFilePath, const std::string& fsFilePath);
    explicit ShaderProgram(const std::string& vsFilePath, const std::string& fsFilePath, const std::string& gsFilePath);
    ShaderProgram(ShaderProgram&& other) noexcept;
    ShaderProgram& operator=(ShaderProgram&& other) noexcept;
    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;
    virtual ~ShaderProgram() = default;

    void assign(const std::string& vsFilePath, const std::string& fsFilePath);
    void assign(const std::string& vsFilePath, const std::string& fsFilePath, const std::string& gsFilePath);
    virtual void recompile() override; // WARNING THIS RESETS UNIFORMS
};

} // namespace mylib