#pragma once

#include "glad/gl.h"

#include "VertexArray.hpp"
#include "Buffer.hpp"
#include "Shader.hpp"
#include "Model.hpp"

#include <cstdint>

namespace mylib
{

enum class Primitive : GLenum
{
    POINTS         = GL_POINTS,           // Each vertex is a single point
    LINES          = GL_LINES,            // Each pair of vertices is a single line (Requires at least 2 vertices)
    LINE_STRIP     = GL_LINE_STRIP,       // Each added vertex forms a new line with the last one (Requires at least 2 vertices)
    LINE_LOOP      = GL_LINE_LOOP,        // LINE_STRIP but last vertex is connected to the first (Requires at least 2 vertices)
    TRIANGLES      = GL_TRIANGLES,        // Each three vertices forms a single triangle (Requires at least 3 vertices)
    TRIANGLE_STRIP = GL_TRIANGLE_STRIP,   // Each added vertex forms a new triangle with the two previous ones (Requires at least 3 vertices)
    TRIANGLE_FAN   = GL_TRIANGLE_FAN      // Each added vertex forms a new triangle with the last vertex and the first (Requires at least 3 vertices)
};

enum class DrawMode : GLenum
{
    FILL      = GL_FILL,   // Renders faces as filled triangles
    WIREFRAME = GL_LINE,   // Renders faces with connected vertices
    NO_FILL   = GL_POINT   // Renders faces with vertices only
};

enum class Face : GLenum
{
    FRONT_AND_BACK = GL_FRONT_AND_BACK,   // Regardles of inside or outside the mesh
    FRONT          = GL_FRONT,            // Usually means outside the mesh
    BACK           = GL_BACK              // Usually means inside the mesh
};

class Renderer
{
public:
    Renderer();

    void draw(const mylib::VertexArray& VAO, GLuint count, const Shader& shader, Primitive primitive = Primitive::POINTS) const;
    void drawIndexed(const mylib::VertexArray& VAO, GLuint count, const Shader& shader, Primitive primitive = Primitive::POINTS) const;

    void draw(const mylib::Mesh& mesh, const Shader& shader, Primitive primitive = Primitive::TRIANGLES) const;
    void drawInstanced(const mylib::Mesh& mesh, const GLuint count, const Shader& shader, Primitive primitive = Primitive::TRIANGLES) const;

    void draw(const mylib::Model& model, const Shader& shader, Primitive primitive = Primitive::TRIANGLES) const;
    void drawInstanced(const mylib::Model& model, const GLuint count, const Shader& shader, Primitive primitive = Primitive::TRIANGLES) const;

    void drawMode(mylib::DrawMode mode) const;
    void drawMode(mylib::Face face, mylib::DrawMode mode) const;
    void backgroundColor(glm::vec4 color) const;
    void backgroundColor(float r, float g, float b, float a) const;
    void clear() const;
private:
    constexpr inline GLenum toGL(Primitive primitive) const {return static_cast<GLenum>(primitive);};
    constexpr inline GLenum toGL(DrawMode mode) const {return static_cast<GLenum>(mode);}
    constexpr inline GLenum toGL(Face face) const {return static_cast<GLenum>(face);}
};

} // namespace mylib