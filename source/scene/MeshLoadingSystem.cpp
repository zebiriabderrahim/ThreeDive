#include <spdlog/spdlog.h>
#include "MeshLoadingSystem.h"
#include "../renderer/RenderCommand.h"

namespace s3Dive {


    void MeshLoadingSystem::loadModel(Scene& scene, const UUID& modelEntityUUID) {
        const auto& modelComponent = scene.getComponent<ModelComponent>(modelEntityUUID);

        std::cout << "Loading model: " << modelComponent.filepath << std::endl;

        Assimp::Importer importer;

        const aiScene* aiScene = importer.ReadFile(modelComponent.filepath,
                                                   aiProcess_Triangulate |
                                                   aiProcess_FlipUVs |
                                                   aiProcess_CalcTangentSpace |
                                                   aiProcess_GenNormals |
                                                   aiProcess_JoinIdenticalVertices |
                                                   aiProcess_SortByPType);

        if (!aiScene || aiScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !aiScene->mRootNode) {
            std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
            return;
        }

        std::cout << "Meshes in scene: " << aiScene->mNumMeshes << std::endl;
        std::cout << "Root node children: " << aiScene->mRootNode->mNumChildren << std::endl;

        processNode(scene, aiScene->mRootNode, aiScene, modelEntityUUID);
    }

    void MeshLoadingSystem::processNode(Scene& scene, const aiNode* node, const aiScene* aiScene, const UUID& modelEntityUUID, int depth) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh const* mesh = aiScene->mMeshes[node->mMeshes[i]];
            MeshComponent meshComponent = processMesh(mesh, aiScene);
            MaterialComponent materialComponent = processMaterial(aiScene->mMaterials[mesh->mMaterialIndex]);

            UUID meshEntityUUID{};
            scene.addComponent<MeshComponent>(meshEntityUUID, std::move(meshComponent));
            scene.addComponent<MaterialComponent>(meshEntityUUID, std::move(materialComponent));

            auto& modelComponent = scene.getComponent<ModelComponent>(modelEntityUUID);
            modelComponent.meshEntities.push_back(meshEntityUUID);
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(scene, node->mChildren[i], aiScene, modelEntityUUID, depth + 1);
        }
    }

    MeshComponent MeshLoadingSystem::processMesh(const aiMesh* mesh, const aiScene* scene) {
        MeshComponent meshComponent;

        std::cout << "Processing mesh: " << mesh->mName.C_Str()
                  << ", Vertices: " << mesh->mNumVertices
                  << ", Faces: " << mesh->mNumFaces << std::endl;

        // Process vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex{};
            vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

            if (mesh->HasNormals()) {
                vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            }

            if (mesh->mTextureCoords[0]) {
                vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            } else {
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            }

            meshComponent.vertices.push_back(vertex);
        }

        // Process indices
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                meshComponent.indices.push_back(face.mIndices[j]);
            }
        }

        // Initialize mesh
        meshComponent.initialize();

        return meshComponent;
    }

    MaterialComponent MeshLoadingSystem::processMaterial(const aiMaterial* material) {
        MaterialComponent materialComponent;

        aiColor3D color;
        float shininess;

        if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
            materialComponent.albedo = glm::vec3(color.r, color.g, color.b);
        }

        if (material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS) {
            materialComponent.roughness = 1.0f - (shininess / 128.0f);
        }

        materialComponent.diffuseTexture = loadMaterialTexture(material, aiTextureType_DIFFUSE);

        return materialComponent;
    }

    std::shared_ptr<GLTexture> MeshLoadingSystem::loadMaterialTexture(const aiMaterial* mat, aiTextureType type) {
        if (mat->GetTextureCount(type) > 0) {
            aiString str;
            mat->GetTexture(type, 0, &str);
            std::string texturePath = "" + std::string(str.C_Str());
            return std::make_shared<GLTexture>(texturePath);
        }
        return std::make_shared<GLTexture>("wall.jpg");
    }
} // s3Dive