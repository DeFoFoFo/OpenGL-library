#pragma once

#include "glad/gl.h"
#include "glm/glm.hpp"

#include "Buffer.hpp"
#include "VertexArray.hpp"
#include "Texture.hpp"

#include <vector>

namespace mylib
{

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 UV;
};

class Mesh
{
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    
    Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Texture>& textures);
    Mesh(Mesh&&) = default;
    Mesh& operator=(Mesh&&) = default;
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;


    const VertexArray& getVAO() const;
    const Buffer& getVBO() const;
    const Buffer& getEBO() const;
private:
    VertexArray m_VAO;
    Buffer m_VBO;
    Buffer m_EBO;

    void setupMesh();
};

} // namespace mylib