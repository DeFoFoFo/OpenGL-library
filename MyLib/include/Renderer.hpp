#pragma once

#include "glad/gl.h"

#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"

#include <cstdint>

namespace mylib
{

enum class Primitive : GLuint
{
    POINTS = GL_POINTS,                   // Each vertex is a single point
    LINES = GL_LINES,                     // Each pair of vertices is a single line
    LINE_STRIP = GL_LINE_STRIP,           // Each added vertex forms a new line with the last one
    LINE_LOOP = GL_LINE_LOOP,             // LINE_STRIP but last vertex is connected to the first
    TRIANGLES = GL_TRIANGLES,             // Each three vertices forms a single triangle
    TRIANGLE_STRIP = GL_TRIANGLE_STRIP,   // Each added vertex forms a new triangle with the two previous ones
    TRIANGLE_FAN = GL_TRIANGLE_FAN        // Each added vertex forms a new triangle with the last vertex and the first
};

class Renderer
{
public:
    void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, Primitive primitive = Primitive::TRIANGLES) const;
    void showLines(bool state);
    void clear() const;
};

} // namespace mylib