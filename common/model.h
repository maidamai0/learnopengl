/**
 * @file model.h
 * @author tonghao.yuan (yuantonghao@gmail.com)
 * @brief
 * @version 0.1
 * @date 2019-12-25
 *
 * @copyright Copyright (c) 2019
 *
 */

#include <string>
#include <vector>

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "common/mesh.h"
#include "common/texture.h"

class Model {
   public:
    Model(std::string&& path, const bool gamma = false) : gammaCorrection{gamma} {
        load_model(std::move(path));
    }
    void Draw(Shader& shader) {
        for (const auto& mesh : meshes_) {
            mesh.Draw(shader);
        }
    }
    ~Model() = default;

   private:
    void load_model(std::string&& path) {
        Assimp::Importer importer;
        const auto scene = importer.ReadFile(
            path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            fmt::print("assimp load model failed:{}\n", importer.GetErrorString());
            return;
        }

        directory_ = path.substr(0, path.find_last_of('/'));
        process_node(scene->mRootNode, scene);
    }

    void process_node(aiNode* node, const aiScene* scene) {
        for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
            auto mesh = scene->mMeshes[node->mMeshes[i]];
            meshes_.push_back(process_mesh(mesh, scene));
        }

        for (unsigned int i = 0; i < node->mNumChildren; ++i) {
            process_node(node->mChildren[i], scene);
        }
    }

    mesh::Mesh process_mesh(aiMesh* mesh, const aiScene* scene) {
        std::vector<mesh::Vertex> vertices;
        std::vector<unsigned int> ids;
        std::vector<mesh::Texture> texs;

        for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
            mesh::Vertex vtx;
            // position
            vtx.Position.x = mesh->mVertices[i].x;
            vtx.Position.y = mesh->mVertices[i].y;
            vtx.Position.z = mesh->mVertices[i].z;

            // normals
            vtx.Normal.x = mesh->mNormals[i].x;
            vtx.Normal.y = mesh->mNormals[i].y;
            vtx.Normal.z = mesh->mNormals[i].z;

            // texcoord
            if (mesh->mTextureCoords[0]) {
                vtx.TexCoords.x = mesh->mTextureCoords[0][i].x;
                vtx.TexCoords.y = mesh->mTextureCoords[0][i].y;
            }

            // Tangents
            vtx.Tangent.x = mesh->mTangents[i].x;
            vtx.Tangent.y = mesh->mTangents[i].y;
            vtx.Tangent.z = mesh->mTangents[i].z;

            // bittangent
            vtx.Bitangent.x = mesh->mBitangents[i].x;
            vtx.Bitangent.y = mesh->mBitangents[i].y;
            vtx.Bitangent.z = mesh->mBitangents[i].z;

            vertices.push_back(vtx);
        }

        // faces
        for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
            auto face = mesh->mFaces[i];
            for (unsigned j = 0; j < face.mNumIndices; ++j) {
                ids.push_back(face.mIndices[j]);
            }
        }

        // material
        if (mesh->mMaterialIndex >= 0) {
            auto material = scene->mMaterials[mesh->mMaterialIndex];
            std::vector<mesh::Texture> diffuse_maps =
                load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            texs.insert(texs.end(), diffuse_maps.begin(), diffuse_maps.end());
            std::vector<mesh::Texture> specular_maps =
                load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
            texs.insert(texs.end(), specular_maps.begin(), specular_maps.end());
            std::vector<mesh::Texture> normal_maps =
                load_material_textures(material, aiTextureType_HEIGHT, "texture_normal");
            texs.insert(texs.end(), normal_maps.begin(), normal_maps.end());
            std::vector<mesh::Texture> height_maps =
                load_material_textures(material, aiTextureType_AMBIENT, "texture_height");
            texs.insert(texs.end(), height_maps.begin(), height_maps.end());
        }

        return {std::move(vertices), std::move(ids), std::move(texs)};
    }

    std::vector<mesh::Texture> load_material_textures(aiMaterial* mat, aiTextureType type,
                                                      std::string type_name) {
        std::vector<mesh::Texture> texs;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i) {
            aiString str;
            mat->GetTexture(type, i, &str);
            bool skip = false;
            for (const auto tex : textures_loaded_) {
                if (tex.path == std::string(str.C_Str())) {
                    skip = true;
                    texs.push_back(tex);
                    break;
                }
            }

            if (!skip) {
                mesh::Texture tex;
                Texture texture(str.C_Str());
                tex.id = texture.GetTextureID();
                tex.type = type_name;
                tex.path = str.C_Str();
                texs.push_back(tex);
            }
        }
        return texs;
    }

   private:
    std::vector<mesh::Mesh> meshes_;
    std::string directory_;
    std::vector<mesh::Texture> textures_loaded_;
    bool gammaCorrection = false;
};
