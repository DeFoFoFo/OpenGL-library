#include "Renderer.hpp"

#include <string>
#include <iostream>

mylib::Renderer::Renderer()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
}

void mylib::Renderer::draw(const mylib::VertexArray &VAO, GLuint count, const Shader &shader, Primitive primitive) const
{
    shader.bind();

    VAO.bind();
    glDrawArrays(toGL(primitive), 0, count);
}

void mylib::Renderer::draw(const mylib::Mesh &mesh, const Shader &shader, Primitive primitive) const
{
    shader.bind();

    uint16_t diffuseNr = 0;
    uint16_t specularNr = 0;
    for (uint32_t i{}; i < mesh.textures.size(); ++i)
    {
        std::string name;
        switch (mesh.textures[i].getTypeName())
        {
            case mylib::TextureType::NONE:
            {
                std::cerr << "MYLIB::ERROR::RENDERER::TEXTURE_HAS_NO_TYPE" << std::endl;
                break;
            }
            case mylib::TextureType::DIFFUSE:
            {
                name = "text_diffuse" + std::to_string(diffuseNr++);
                break;
            }
            case mylib::TextureType::SPECULAR:
            {
                name = "text_specular" + std::to_string(specularNr++);
                break;
            }
            default:
                std::cerr << "MYLIB::ERROR::RENDERER::UNSUPPORTED_TEXTURE" << std::endl;
        }

        glUniform1i(glGetUniformLocation(shader.getID(), ("material." + name).c_str()), i);
        mesh.textures[i].bind(i);
    }

    mesh.getVAO().bind();
    
    glDrawElements(toGL(primitive), mesh.indices.size(), GL_UNSIGNED_INT, 0);
}

void mylib::Renderer::draw(const mylib::Model &model, const Shader &shader, Primitive primitive) const
{
    const std::vector<mylib::Mesh>& meshes = model.getMeshes();
    for (const auto& mesh : meshes)
    {
        draw(mesh, shader, primitive);
    }
}

void mylib::Renderer::wireframe(bool state) const
{
    glPolygonMode(GL_FRONT_AND_BACK, (state) ? GL_LINE : GL_FILL);
}

void mylib::Renderer::backgroundColor(glm::vec4 color) const
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void mylib::Renderer::backgroundColor(float r, float g, float b, float a) const
{
    glClearColor(r, g, b, a);
}

void mylib::Renderer::clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

constexpr GLenum mylib::Renderer::toGL(Primitive primitive) const
{
    return static_cast<GLenum>(primitive);
}
