#include "RenderSystem.h"
#include "components.h"
#include "../renderer/RenderCommand.h"
#include <glm/gtc/type_ptr.hpp>

namespace s3Dive {

    void RenderSystem::render(Scene& scene, GLShaderProgram& shaderProgram, const CameraController& cameraController) {
        shaderProgram.use();

        // Set up global light
        shaderProgram.updateShaderUniform("lightColor", glm::vec3(1.0f, 1.0f, 1.0f)); // White light
        shaderProgram.updateShaderUniform("ambientStrength", 0.7f); // Stronger ambient for more uniform lighting

        // Set view position (camera position)
        const auto& camera = cameraController.getCamera();
        shaderProgram.updateShaderUniform("viewPos", cameraController.getDistanceToTarget());

        glm::mat4 projectionMatrix = camera.getProjectionMatrix();
        glm::mat4 viewMatrix = camera.getViewMatrix();
        shaderProgram.updateShaderUniform("view", glm::value_ptr(viewMatrix));
        shaderProgram.updateShaderUniform("projection", glm::value_ptr(projectionMatrix));

        // Iterate through all entities with a ModelComponent
        auto modelView = scene.view<ModelComponent>();
        for (auto modelEntity : modelView) {
            const auto& modelComponent = modelView.get<ModelComponent>(modelEntity);

            // Render each mesh entity associated with the model
            for (const auto& meshEntityUUID : modelComponent.meshEntities) {
                if (!scene.hasComponent<TransformComponent>(meshEntityUUID) ||
                    !scene.hasComponent<MeshComponent>(meshEntityUUID) ||
                    !scene.hasComponent<MaterialComponent>(meshEntityUUID)) {
                    continue;
                }

                const auto& transform = scene.getComponent<TransformComponent>(meshEntityUUID);
                const auto& mesh = scene.getComponent<MeshComponent>(meshEntityUUID);
                const auto& material = scene.getComponent<MaterialComponent>(meshEntityUUID);

                if (!mesh.isInitialized || !mesh.vertexArray) {
                    continue;
                }

                // Set model matrix using the TransformComponent's transformation matrix
                glm::mat4 model = transform.GetTransform();
                shaderProgram.updateShaderUniform("model", glm::value_ptr(model));
                shaderProgram.updateShaderUniform("albedo", material.albedo);
//                shaderProgram.updateShaderUniform("metallic", material.metallic);
//                shaderProgram.updateShaderUniform("roughness", material.roughness);

                if (material.diffuseTexture) {
                    material.diffuseTexture->bind(0);
                    shaderProgram.updateShaderUniform("material.diffuseTexture", 0);
                }

                mesh.vertexArray->bind();
                RenderCommand::drawIndexed(*mesh.vertexArray, static_cast<GLint>(mesh.indices.size()));
                mesh.vertexArray->unbind();
            }
        }

        shaderProgram.unuse();
    }

} // namespace s3Dive
