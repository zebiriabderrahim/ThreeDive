#include "MeshLoadingSystem.h"
#include "../renderer/RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>

namespace s3Dive {

    void ModelLoadingSystem::loadModel(Scene& scene, const std::string& filepath) {

        spdlog::info("Loading model: {}", filepath);

        Assimp::Importer importer;
        constexpr unsigned int importFlags =
                aiProcess_Triangulate |
                aiProcess_FlipUVs |
                aiProcess_CalcTangentSpace |
                aiProcess_GenNormals |
                aiProcess_JoinIdenticalVertices |
                aiProcess_SortByPType;

        const aiScene* aiScene = importer.ReadFile(filepath, importFlags);

        if (!aiScene || aiScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !aiScene->mRootNode) {
//            throw std::runtime_error(fmt::format("ERROR::ASSIMP::{}", importer.GetErrorString()));
            return;
        }

        spdlog::info("Meshes in scene: {}", aiScene->mNumMeshes);
        spdlog::info("Root node children: {}", aiScene->mRootNode->mNumChildren);

        auto modelEntity = scene.createEntity();
        auto modelEntityUUID = scene.getEntityUUID(modelEntity).value();
        scene.addComponent<ModelComponent>(modelEntityUUID);

        processNode(scene, aiScene->mRootNode, aiScene, modelEntityUUID, glm::mat4(1.0f));
    }

    void ModelLoadingSystem::processNode(Scene& scene, const aiNode* node, const aiScene* aiScene,
                                         const UUID& modelEntityUUID, const glm::mat4& parentTransform) {
        // Extract the transformation matrix from the node and combine it with the parent transform
        aiMatrix4x4 aiTransform = node->mTransformation;
        aiTransform.Transpose(); // Assimp uses row-major matrices, we need column-major for glm

        glm::mat4 nodeTransform = parentTransform * glm::make_mat4(&aiTransform.a1);

        // Apply scaling only at the root node
        if (node == aiScene->mRootNode) {
            // Apply scaling
            nodeTransform = glm::scale(nodeTransform, glm::vec3(0.01f));
            nodeTransform = glm::rotate(nodeTransform, glm::radians(+90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            auto translationAdjustment = glm::vec3(0.0f, 0.0f, 0.0f); // Adjust as needed
            nodeTransform = glm::translate(nodeTransform, translationAdjustment);
        }
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            const aiMesh* mesh = aiScene->mMeshes[node->mMeshes[i]];
            MeshComponent meshComponent = processMesh(mesh);
            MaterialComponent materialComponent = processMaterial(aiScene->mMaterials[mesh->mMaterialIndex]);

            auto meshEntity = scene.createEntity();
            auto meshEntityUUID = scene.getEntityUUID(meshEntity).value();
            scene.addComponent<MeshComponent>(meshEntityUUID, std::move(meshComponent));
            scene.addComponent<MaterialComponent>(meshEntityUUID, std::move(materialComponent));

            // Extract translation, rotation, and scale from the nodeTransform
            glm::vec3 translation;
            glm::vec3 rotation;
            glm::vec3 scale;
            glm::quat orientation;
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::decompose(nodeTransform, scale, orientation, translation, skew, perspective);
            rotation = glm::eulerAngles(orientation);

            // Create TransformComponent with the applied transformation
            scene.addComponent<TransformComponent>(meshEntityUUID, TransformComponent{
                    translation,
                    rotation,
                    scale // Scale down the model
            });

            auto& modelComponent = scene.getComponent<ModelComponent>(modelEntityUUID);
            modelComponent.meshEntities.push_back(meshEntityUUID);
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(scene, node->mChildren[i], aiScene, modelEntityUUID, nodeTransform);
        }
    }
    MeshComponent ModelLoadingSystem::processMesh(const aiMesh* mesh) {
        MeshComponent meshComponent;

        spdlog::info("Processing mesh: {}, Vertices: {}, Faces: {}",
                     mesh->mName.C_Str(), mesh->mNumVertices, mesh->mNumFaces);

        meshComponent.vertices.reserve(mesh->mNumVertices);
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

        meshComponent.indices.reserve(mesh->mNumFaces * 3);
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            const aiFace& face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                meshComponent.indices.push_back(face.mIndices[j]);
            }
        }

        initializeMeshComponent(meshComponent);

        return meshComponent;
    }

    MaterialComponent ModelLoadingSystem::processMaterial(const aiMaterial* material) const {
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

    std::shared_ptr<GLTexture> ModelLoadingSystem::loadMaterialTexture(const aiMaterial* mat, aiTextureType type) {
        if (mat->GetTextureCount(type) > 0) {
            aiString str;
            mat->GetTexture(type, 0, &str);
            std::string texturePath = str.C_Str();
            return std::make_shared<GLTexture>(texturePath);
        }
        return std::make_shared<GLTexture>("wall.jpg");
    }

    void ModelLoadingSystem::initializeMeshComponent(MeshComponent& meshComponent) {
        std::vector<float> vertexData;
        vertexData.reserve(meshComponent.vertices.size() * 8);
        for (const auto& vertex : meshComponent.vertices) {
            vertexData.insert(vertexData.end(), {
                    vertex.Position.x, vertex.Position.y, vertex.Position.z,
                    vertex.Normal.x, vertex.Normal.y, vertex.Normal.z,
                    vertex.TexCoords.x, vertex.TexCoords.y
            });
        }

        auto vertexBuffer = std::make_shared<GLVertexBuffer>(vertexData);
        GLVertexBufferLayout layout;
        layout.addVertexElement<float>(3); // Position
        layout.addVertexElement<float>(3); // Normal
        layout.addVertexElement<float>(2); // TexCoords
        vertexBuffer->setLayout(layout);

        meshComponent.vertexArray = std::make_shared<GLVertexArray>();
        meshComponent.vertexArray->addVertexBuffer(vertexBuffer);

        auto indexBuffer = std::make_shared<GLIndexBuffer>(meshComponent.indices);
        meshComponent.vertexArray->setIndexBuffer(indexBuffer);

        meshComponent.isInitialized = true;
    }

} // namespace s3Dive
