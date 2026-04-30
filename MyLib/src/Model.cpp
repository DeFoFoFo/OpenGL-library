#include "Model.hpp"

#include "GLFW/glfw3.h"

#include <iostream>

mylib::Model::Model(const char* path)
{
#ifdef MYLIB_DEBUG
    float startTime = glfwGetTime();
#endif

    loadModel(path);

#ifdef MYLIB_DEBUG
    float loadTime = glfwGetTime() - startTime;
    std::cout << "MYLIB::MODEL::LOADED_IN " << loadTime * 1000 << "ms\n"
        << "\tPATH: " << path << std::endl;
#endif
}

const std::vector<mylib::Mesh>& mylib::Model::getMeshes() const
{
    return m_meshes;
}

void mylib::Model::loadModel(std::string path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "MYLIB::ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    m_directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void mylib::Model::processNode(const aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

mylib::Mesh mylib::Model::processMesh(const aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        // process vertex positions, normals and texture coordinates
        glm::vec3 vector;

        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;

        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.UV = vec;
        }
        else
            vertex.UV = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // process material
    if (scene->HasMaterials())
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<mylib::Texture> diffuseMaps = loadMaterialTextures(scene, material,
            aiTextureType_DIFFUSE);
        textures.insert(textures.end(),
            std::make_move_iterator(diffuseMaps.begin()),
            std::make_move_iterator(diffuseMaps.end()));

        std::vector<mylib::Texture> specularMaps = loadMaterialTextures(scene, material,
            aiTextureType_SPECULAR);
        textures.insert(textures.end(),
            std::make_move_iterator(specularMaps.begin()),
            std::make_move_iterator(specularMaps.end()));
    }

    return mylib::Mesh(std::move(vertices), std::move(indices), std::move(textures));

}

std::vector<mylib::Texture> mylib::Model::loadMaterialTextures(const aiScene* scene, const aiMaterial* material, const aiTextureType type)
{
    std::vector<mylib::Texture> textures;
    for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString str;
        material->GetTexture(type, i, &str);
        mylib::Texture texture;

        if (str.C_Str()[0] == '*') // Embedded texture
        {
            int index = atoi(str.C_Str() + 1);
            aiTexture* tex = scene->mTextures[index];

            unsigned char* data = reinterpret_cast<unsigned char*>(tex->pcData);
            if (tex->mHeight == 0) // Compressed data (ex: PNG, JPEG)
            {
                size_t size = tex->mWidth;
                texture.loadTexture(mylib::TextureDimension::DIM2, data, size);
            }
            else // Uncompressed data (ex: RAW)
            {
                texture.loadTexture(mylib::TextureDimension::DIM2, tex->mWidth, tex->mHeight, data, GL_BGRA);
            }
        }
        else // Separate texture (ex: OBJ)
        {
            std::string fileName = m_directory + '/' + str.C_Str();
            texture.loadTexture(mylib::TextureDimension::DIM2, fileName.c_str());
        }

        texture.setTypeName(static_cast<mylib::TextureType>(type));
        textures.push_back(std::move(texture));
    }

    return textures;
}