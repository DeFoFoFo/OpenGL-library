#include "Mesh.hpp"

mylib::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<mylib::Texture> textures)
    : vertices{ std::move(vertices) }, indices{ std::move(indices) }, textures{ std::move(textures) }
{
    setupMesh();
}

void mylib::Mesh::setupMesh()
{
    m_VAO.bind();

    mylib::VertexBufferLayout layout;
    layout.push(3, GL_FLOAT);
    layout.push(3, GL_FLOAT);
    layout.push(2, GL_FLOAT);

    m_VBO.fill(mylib::BufferTarget::VBO, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    m_VAO.addBuffer(m_VBO, layout);

    m_EBO.fill(mylib::BufferTarget::EBO, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
    m_EBO.bindAs(mylib::BufferTarget::EBO);
}
