#include "Renderer.hpp"

// To implement
/*
void mylib::Renderer::draw(const VertexArray &va, const Buffer &ib, const Shader &shader, Primitive primitive) const
{
    shader.bind();
    va.bind();
    ib.bindAs(mylib::BufferTarget::IBO);

    glDrawElements(toGL(primitive), ib.getCount(), GL_UNSIGNED_INT, nullptr);
}
*/

void mylib::Renderer::showLines(bool state)
{
    glPolygonMode(GL_FRONT_AND_BACK, (state) ? GL_LINE : GL_FILL);
}

void mylib::Renderer::clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

constexpr GLenum mylib::Renderer::toGL(Primitive primitive) const
{
    return static_cast<GLenum>(primitive);
}
