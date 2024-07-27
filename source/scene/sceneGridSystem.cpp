#include <spdlog/spdlog.h>
#include <bitset>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "sceneGridSystem.h"
#include "../renderer/RenderCommand.h"
#include "../renderer/Renderer.h"


namespace s3Dive {

    void SceneGridSystem::initialize(SceneGridComponent &grid) {
        auto vbo = std::make_shared<GLVertexBuffer>(grid.vertices);
        GLVertexBufferLayout layout{};

        vao_.bind();
        layout.addVertexElement<float>(3);
        layout.addVertexElement<float>(4);
        layout.addVertexElement<float>(1);
        vbo->setLayout(layout);
        vao_.addVertexBuffer(vbo);

        grid.isInitialized = true;
    }

    bool SceneGridSystem::shouldRecalculateVisibility(float currentDistance) const {
        return cachedDistanceToTarget_ < 0.0f ||
               std::abs(currentDistance - cachedDistanceToTarget_) > kDistanceThreshold;
    }

    void SceneGridSystem::calculateVisibility(float distanceToTarget) {
        cachedDetailVisibility1_ = 1.0f - glm::clamp((distanceToTarget - kDetailVisibilityNear1)
                                                     / (kDetailVisibilityFar1 - kDetailVisibilityNear1), 0.0f, 1.0f);
        cachedDetailVisibility2_ = 1.0f - glm::clamp((distanceToTarget - kDetailVisibilityNear2)
                                                     / (kDetailVisibilityFar2 - kDetailVisibilityNear2), 0.0f, 1.0f);
        cachedDistanceToTarget_ = distanceToTarget;
    }

    void SceneGridSystem::update(Scene &scene, float deltaTime) {
        // Any non-rendering logic, if needed
    }

    void
    SceneGridSystem::render(Scene &scene, GLShaderProgram &shaderProgram, const CameraController &cameraController) {
        auto view = scene.view<SceneGridComponent>();
        for (auto entity: view) {
            auto &grid = view.get<SceneGridComponent>(entity);
            renderGrid(grid, shaderProgram, cameraController);
        }
    }

    void SceneGridSystem::renderGrid(SceneGridComponent &grid, GLShaderProgram &shaderProgram,
                                     const CameraController &cameraController) {
        if (!grid.isInitialized) {
            initialize(grid);
        }

        if (float currentDistanceToTarget = cameraController.getDistanceToTarget();
                shouldRecalculateVisibility(currentDistanceToTarget)) {
            calculateVisibility(currentDistanceToTarget);
        }

        shaderProgram.use();
        shaderProgram.updateShaderUniform("detailVisibility1", cachedDetailVisibility1_);
        shaderProgram.updateShaderUniform("detailVisibility2", cachedDetailVisibility2_);

        RenderCommand::drawLines(vao_, static_cast<GLint>(grid.vertices.size() / 8));

        shaderProgram.unuse();
    }

    void MeshLoadingSystem::update(Scene &scene, float deltaTime) {
        // This method could be used to check for any new models that need loading
        // For now, we'll leave it empty as loading will be triggered manually
    }

    void MeshLoadingSystem::loadModel(Scene &scene, const UUID &modelEntityUUID) {
        const auto &modelComponent = scene.getComponent<ModelComponent>(modelEntityUUID);

        std::cout << "Loading model: " << modelComponent.filepath << std::endl;

        Assimp::Importer importer;

        const aiScene *aiScene = importer.ReadFile(modelComponent.filepath,
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

    void MeshLoadingSystem::processNode(Scene &scene, aiNode *node, const aiScene *aiScene, const UUID &modelEntityUUID,
                                        int depth) {
        std::cout << std::string(depth * 2, ' ') << "Processing node: "
                  << (node->mName.length > 0 ? node->mName.C_Str() : "Unnamed")
                  << ", Meshes: " << node->mNumMeshes << std::endl;

        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = aiScene->mMeshes[node->mMeshes[i]];
            MeshComponent meshComponent = processMesh(mesh, aiScene);

            UUID meshEntityUUID{};
            std::cout << "Created mesh entity: " << meshEntityUUID << " for model: " << modelEntityUUID << std::endl;

            scene.addComponent<MeshComponent>(meshEntityUUID, std::move(meshComponent));

            auto &modelComponent = scene.getComponent<ModelComponent>(modelEntityUUID);
            modelComponent.meshEntities.push_back(meshEntityUUID);
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(scene, node->mChildren[i], aiScene, modelEntityUUID, depth + 1);
        }
    }

    MeshComponent MeshLoadingSystem::processMesh(aiMesh *mesh, const aiScene *scene) {
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

        // Process material
        if (mesh->mMaterialIndex >= 0) {
            const aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            meshComponent.texture = loadMaterialTexture(material, aiTextureType_DIFFUSE);
        }

        initialize(meshComponent);

        return meshComponent;
    }

    std::shared_ptr<GLTexture> MeshLoadingSystem::loadMaterialTexture(const aiMaterial* mat, aiTextureType type) {
        // Debug output
        std::cout << "Attempting to load texture of type: " << type << std::endl;
        std::cout << "Texture count: " << mat->GetTextureCount(type) << std::endl;

        // Check for diffuse textures
        if (mat->GetTextureCount(type) > 0) {
            aiString str;
            mat->GetTexture(type, 0, &str);
            std::cout << "Found texture path: " << str.C_Str() << std::endl;
            return std::make_shared<GLTexture>(str.C_Str());
        }

        // If no diffuse texture, check for ambient textures
        if (type == aiTextureType_DIFFUSE && mat->GetTextureCount(aiTextureType_AMBIENT) > 0) {
            aiString str;
            mat->GetTexture(aiTextureType_AMBIENT, 0, &str);
            std::cout << "Found ambient texture path: " << str.C_Str() << std::endl;
            return std::make_shared<GLTexture>(str.C_Str());
        }

        // If still no texture found, use a default texture
        std::cout << "No texture found, using default texture." << std::endl;
        return std::make_shared<GLTexture>("wall.jpg");
    }

    void MeshLoadingSystem::initialize(MeshComponent &meshComponent) {
        // Convert Vertex struct to a flat array of floats
        std::vector<float> vertexData;
        for (const auto &vertex: meshComponent.vertices) {
            // Position
            vertexData.push_back(vertex.Position.x);
            vertexData.push_back(vertex.Position.y);
            vertexData.push_back(vertex.Position.z);
            // Normal
            vertexData.push_back(vertex.Normal.x);
            vertexData.push_back(vertex.Normal.y);
            vertexData.push_back(vertex.Normal.z);
            // TexCoords
            vertexData.push_back(vertex.TexCoords.x);
            vertexData.push_back(vertex.TexCoords.y);
        }

        // Create and set up the vertex buffer
        meshComponent.vertexBuffer = std::make_shared<GLVertexBuffer>(vertexData);
        GLVertexBufferLayout layout;
        layout.addVertexElement<float>(3); // Position
        layout.addVertexElement<float>(3); // Normal
        layout.addVertexElement<float>(2); // TexCoords
        meshComponent.vertexBuffer->setLayout(layout);

        // Create and set up the vertex array
        meshComponent.vertexArray = std::make_shared<GLVertexArray>();
        meshComponent.vertexArray->addVertexBuffer(meshComponent.vertexBuffer);

        // Set up the index buffer
        auto indexBuffer = std::make_shared<GLIndexBuffer>(meshComponent.indices);
        meshComponent.vertexArray->setIndexBuffer(indexBuffer);

        meshComponent.isInitialized = true;
    }

    void MeshLoadingSystem::render(Scene& scene, GLShaderProgram& shaderProgram) {
        auto view = scene.view<MeshComponent>();
        for (auto entity : view) {
            const auto& mesh = view.get<MeshComponent>(entity);

            if (!mesh.isInitialized || !mesh.vertexArray) {
                continue;
            }

            shaderProgram.use();
            shaderProgram.updateShaderUniform("useTexture", mesh.texture != nullptr);

            if (mesh.texture) {
                mesh.texture->bind();
            }

            mesh.vertexArray->bind();

            // Solid rendering
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            RenderCommand::drawIndexed(*mesh.vertexArray , mesh.indices.size());


            mesh.vertexArray->unbind();
            shaderProgram.unuse();
        }
    }

} // namespace s3Dive
