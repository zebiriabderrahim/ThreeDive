#include <spdlog/spdlog.h>
#include "MeshLoadingSystem.h"
#include "../renderer/RenderCommand.h"

namespace s3Dive {

    void MeshLoadingSystem::update(Scene &scene, float deltaTime) {
        // This method could be used to check for any new models that need loading
        // For now, we'll leave it empty as loading will be triggered manually
    }

    void MeshLoadingSystem::loadModel(Scene &scene, const UUID &modelEntityUUID) {
        const auto &modelComponent = scene.getComponent<ModelComponent>(modelEntityUUID);
        std::cout << "Loading model: " << modelComponent.filepath << std::endl;

        Assimp::Importer importer;
        const aiScene *aiScene = importer.ReadFile(modelComponent.filepath, aiProcess_Triangulate | aiProcess_FlipUVs);
        if (!aiScene || aiScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !aiScene->mRootNode) {
            std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
            return;
        }

        std::cout << "Meshes in scene: " << aiScene->mNumMeshes << std::endl;
        std::cout << "Root node children: " << aiScene->mRootNode->mNumChildren << std::endl;

        clearExistingMeshes(scene, modelEntityUUID);
        processNode(scene, aiScene->mRootNode, aiScene, modelEntityUUID);
    }

    void MeshLoadingSystem::clearExistingMeshes(Scene &scene, const UUID &modelEntityUUID) {
        auto &modelComponent = scene.getComponent<ModelComponent>(modelEntityUUID);
        std::cout << "Clearing existing meshes for model: " << modelEntityUUID << std::endl;
        for (const auto &meshUUID: modelComponent.meshEntities) {
            scene.destroyEntity(meshUUID);
        }
        modelComponent.meshEntities.clear();
    }

    void MeshLoadingSystem::processNode(Scene &scene, aiNode *node, const aiScene *aiScene, const UUID &modelEntityUUID, int depth) {
        std::cout << std::string(depth * 2, ' ') << "Processing node: "
                  << (node->mName.length > 0 ? node->mName.C_Str() : "Unnamed")
                  << ", Meshes: " << node->mNumMeshes << std::endl;

        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = aiScene->mMeshes[node->mMeshes[i]];
            MeshComponent meshComponent = processMesh(mesh, aiScene);
            UUID meshEntityUUID{};
            std::cout << "Created mesh entity: " << meshEntityUUID << " for model: " << modelEntityUUID << std::endl;
            scene.addComponent<MeshComponent>(meshEntityUUID, meshComponent);
            auto &modelComponent = scene.getComponent<ModelComponent>(modelEntityUUID);
            modelComponent.meshEntities.push_back(meshEntityUUID);
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(scene, node->mChildren[i], aiScene, modelEntityUUID, depth + 1);
        }
    }

    MeshComponent MeshLoadingSystem::processMesh(aiMesh *mesh, const aiScene *scene) {
        MeshComponent meshComponent;
        spdlog::info("Processing mesh with {} vertices and {} indices", mesh->mNumVertices, mesh->mNumFaces);

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

        // Process material
        if (mesh->mMaterialIndex >= 0) {
            const aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            meshComponent.texture = loadMaterialTexture(material, aiTextureType_DIFFUSE);
        }

        meshComponent.ambientColor = glm::vec3(0.1f, 0.1f, 0.1f);
        meshComponent.diffuseColor = glm::vec3(0.8f, 0.8f, 0.8f);
        meshComponent.specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
        meshComponent.shininess = 32.0f;

        // Process material if available
        if (mesh->mMaterialIndex >= 0) {
            const aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

            // Load texture
            meshComponent.texture = loadMaterialTexture(material, aiTextureType_DIFFUSE);

            // Load material colors if available
            aiColor3D color(0.f, 0.f, 0.f);
            if (material->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS)
                meshComponent.ambientColor = glm::vec3(color.r, color.g, color.b);
            if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
                meshComponent.diffuseColor = glm::vec3(color.r, color.g, color.b);
            if (material->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
                meshComponent.specularColor = glm::vec3(color.r, color.g, color.b);

            float shininess;
            if (material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
                meshComponent.shininess = shininess;
        }

        initializeMeshForRendering(meshComponent);
        return meshComponent;
    }

    std::shared_ptr<GLTexture> MeshLoadingSystem::loadMaterialTexture(const aiMaterial* mat, aiTextureType type) {
        std::cout << "Attempting to load texture of type: " << type << std::endl;
        std::cout << "Texture count: " << mat->GetTextureCount(type) << std::endl;

        if (mat->GetTextureCount(type) > 0) {
            aiString str;
            mat->GetTexture(type, 0, &str);
            std::cout << "Found texture path: " << str.C_Str() << std::endl;
            return std::make_shared<GLTexture>(str.C_Str());
        }

        if (type == aiTextureType_DIFFUSE && mat->GetTextureCount(aiTextureType_AMBIENT) > 0) {
            aiString str;
            mat->GetTexture(aiTextureType_AMBIENT, 0, &str);
            std::cout << "Found ambient texture path: " << str.C_Str() << std::endl;
            return std::make_shared<GLTexture>(str.C_Str());
        }

        std::cout << "No texture found, using default texture." << std::endl;
        return std::make_shared<GLTexture>("wall.jpg");
    }

    void MeshLoadingSystem::initializeMeshForRendering(MeshComponent &meshComponent) {
        std::vector<float> vertexData;
        for (const auto &vertex : meshComponent.vertices) {
            vertexData.push_back(vertex.Position.x);
            vertexData.push_back(vertex.Position.y);
            vertexData.push_back(vertex.Position.z);
            vertexData.push_back(vertex.Normal.x);
            vertexData.push_back(vertex.Normal.y);
            vertexData.push_back(vertex.Normal.z);
            vertexData.push_back(vertex.TexCoords.x);
            vertexData.push_back(vertex.TexCoords.y);
        }

        meshComponent.vertexBuffer = std::make_shared<GLVertexBuffer>(vertexData);
        GLVertexBufferLayout layout;
        layout.addVertexElement<float>(3); // Position
        layout.addVertexElement<float>(3); // Normal
        layout.addVertexElement<float>(2); // TexCoords
        meshComponent.vertexBuffer->setLayout(layout);

        meshComponent.vertexArray = std::make_shared<GLVertexArray>();
        meshComponent.vertexArray->addVertexBuffer(meshComponent.vertexBuffer);

        auto indexBuffer = std::make_shared<GLIndexBuffer>(meshComponent.indices);
        meshComponent.vertexArray->setIndexBuffer(indexBuffer);

        meshComponent.isInitialized = true;
    }

} // s3Dive