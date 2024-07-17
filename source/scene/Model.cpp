//
// Created by ABDERRAHIM ZEBIRI on 2024-07-14.
//

#include <spdlog/spdlog.h>
#include "Model.h"

namespace s3Dive {

    Model::Model() = default;

    bool Model::LoadModel(const std::string &path) {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            spdlog::error("ERROR::ASSIMP::{}", importer.GetErrorString());
            return false;
        }
        m_Directory = path.substr(0, path.find_last_of('/'));

        ProcessNode(scene->mRootNode, scene);
        return true;
    }

    void Model::ProcessNode(aiNode *node, const aiScene *scene) {
        // Process all the node's meshes (if any)
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            m_Meshes.push_back(ProcessMesh(mesh, scene));
        }
        // Then do the same for each of its children
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            ProcessNode(node->mChildren[i], scene);
        }
    }

    Model::Mesh Model::ProcessMesh(aiMesh *mesh, const aiScene *scene) {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        // Process vertex positions, normals and texture coordinates
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            vertices.push_back(mesh->mVertices[i].x);
            vertices.push_back(mesh->mVertices[i].y);
            vertices.push_back(mesh->mVertices[i].z);

            if (mesh->HasNormals()) {
                vertices.push_back(mesh->mNormals[i].x);
                vertices.push_back(mesh->mNormals[i].y);
                vertices.push_back(mesh->mNormals[i].z);
            }

            if (mesh->mTextureCoords[0]) {
                vertices.push_back(mesh->mTextureCoords[0][i].x);
                vertices.push_back(mesh->mTextureCoords[0][i].y);
            } else {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }
        }

        // Process indices
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        // Create OpenGL objects
        Mesh glMesh;
        glMesh.vao = std::make_unique<GLVertexArray>();

        glMesh.vbo = std::make_shared<GLVertexBuffer>(vertices);
        GLVertexBufferLayout layout;
        layout.addVertexElement<float>(3); // 3 floats for position
        layout.addVertexElement<float>(3); // 3 floats for normal
        layout.addVertexElement<float>(2); // 2 floats for texture coordinates
        glMesh.vbo->setLayout(layout);

        glMesh.vao->addVertexBuffer(glMesh.vbo);

        glMesh.ibo = std::make_unique<GLIndexBuffer>(indices);
        glMesh.indexCount = static_cast<unsigned int>(indices.size());

        // Process material
        if (mesh->mMaterialIndex >= 0) {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            std::vector<std::shared_ptr<GLTexture>> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE,
                                                                                       "texture_diffuse");
            glMesh.textures.insert(glMesh.textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        }

        return glMesh;
    }

    std::vector<std::shared_ptr<GLTexture>>
    Model::LoadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string &typeName) {
        std::vector<std::shared_ptr<GLTexture>> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type, i, &str);

            // Check if texture was loaded before
            bool skip = false;
//            for (unsigned int j = 0; j < m_LoadedTextures.size(); j++) {
//                if (std::strcmp(m_LoadedTextures[j]->getPath().data(), str.C_Str()) == 0) {
//                    textures.push_back(m_LoadedTextures[j]);
//                    skip = true;
//                    break;
//                }
//            }
            if (!skip) {
                auto texture = std::make_shared<GLTexture>(m_Directory + '/' + str.C_Str());
                textures.push_back(texture);
                m_LoadedTextures.push_back(texture);
            }
        }
        return textures;
    }

     void Model::Draw( GLShaderProgram &shader) const {
        for (const auto &mesh: m_Meshes) {
            unsigned int diffuseNr = 1;
            unsigned int specularNr = 1;

            for (unsigned int i = 0; i < mesh.textures.size(); i++) {
                mesh.textures[i]->bind(i);

                std::string number;
                if (std::string name = "texture_diffuse"; name == "texture_diffuse"){
                    diffuseNr++;
                    number = std::to_string(diffuseNr);
                }
                else if (name == "texture_specular"){
                    specularNr++;
                    number = std::to_string(specularNr);
                }

                shader.updateShaderUniform("detailVisibility1", 1);
            }

            mesh.vao->bind();
            mesh.ibo->bind();
            glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
        }
    }
} // s3Dive