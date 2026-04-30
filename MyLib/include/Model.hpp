#pragma once

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Mesh.hpp"
#include "Texture.hpp"

namespace mylib
{

class Model
{
public:
    Model(const char* path);

    const std::vector<mylib::Mesh>& getMeshes() const;
private:
    std::vector<Mesh> m_meshes;
    std::string m_directory;

    void loadModel(std::string path);
    void processNode(const aiNode* node, const aiScene* scene);
    mylib::Mesh processMesh(const aiMesh* mesh, const aiScene* scene);

    std::vector<mylib::Texture> loadMaterialTextures(const aiScene* scene, const aiMaterial* material, const aiTextureType type);
};

} // namespace mylib