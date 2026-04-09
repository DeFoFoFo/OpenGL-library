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
    
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
    Mesh(Mesh&&) = default;
    Mesh& operator=(Mesh&&) = default;
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;


    constexpr inline const VertexArray& getVAO() const {return m_VAO;}
    constexpr inline const Buffer& getVBO() const {return m_VBO;}
    constexpr inline const Buffer& getEBO() const {return m_EBO;}
private:
    VertexArray m_VAO;
    Buffer m_VBO;
    Buffer m_EBO;

    void setupMesh();
};

} // namespace mylib