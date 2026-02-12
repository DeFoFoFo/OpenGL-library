#pragma once

#include "glad/gl.h"

#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"

#include <cstdint>

namespace mylib
{

enum class Primitive : uint32_t
{
    TRIANGLES = GL_TRIANGLES,
    TRIANGLE_FAN = GL_TRIANGLE_FAN,
    POINTS = GL_POINTS,
    TRIANGLE_STRIP = GL_TRIANGLE_STRIP
};

class Renderer
{
public:
    void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, Primitive primitive = Primitive::TRIANGLES) const;
    void showLines(bool state);
    void clear() const;
};

} // namespace mylib