#include "RenderSystem.h"
#include "components.h"
#include "../renderer/RenderCommand.h"

namespace s3Dive {
    void RenderSystem::render(Scene& scene, GLShaderProgram& shaderProgram, const CameraController& cameraController) {
        shaderProgram.use();

        // Set up global light
        shaderProgram.updateShaderUniform("lightColor", glm::vec3(1.0f, 1.0f, 1.0f)); // White light
        shaderProgram.updateShaderUniform("ambientStrength", 0.7f); // Stronger ambient for more uniform lighting

        // Set default material properties
        shaderProgram.updateShaderUniform("albedo", glm::vec3(0.435f, 0.435f, 0.435f)); // Neutral gray

        // Set view position (camera position)
        const auto& camera = cameraController.getCamera();
        shaderProgram.updateShaderUniform("viewPos", cameraController.getDistanceToTarget());

        auto view = scene.view<MeshComponent, MaterialComponent>();
        for (auto entity : view) {
            const auto& [mesh, material] = view.get<MeshComponent, MaterialComponent>(entity);

            if (!mesh.isInitialized || !mesh.vertexArray) {
                continue;
            }

            // Set model matrix (assuming you have a TransformComponent)
            glm::mat4 model = glm::mat4(1.0f); // Replace with actual model matrix if available
            glm::mat4 projectionMatrix = camera.getProjectionMatrix();
            glm::mat4 viewMatrix = camera.getViewMatrix();

            shaderProgram.updateShaderUniform("model", glm::value_ptr(model));
            shaderProgram.updateShaderUniform("view", glm::value_ptr(viewMatrix));
            shaderProgram.updateShaderUniform("projection", glm::value_ptr(projectionMatrix));

            mesh.vertexArray->bind();
            RenderCommand::drawIndexed(*mesh.vertexArray, static_cast<GLint>(mesh.indices.size()));
            mesh.vertexArray->unbind();
        }

        shaderProgram.unuse();
    }




    void RenderSystem::setupLights(Scene& scene, GLShaderProgram& shaderProgram) const {
        int lightCount = 0;

        auto lightView = scene.view<LightComponent>();
        for (auto entity : lightView) {
            const auto& light = lightView.get<LightComponent>(entity);
            lightCount++;
            std::string lightName = "lights[" + std::to_string(lightCount) + "]";

            shaderProgram.updateShaderUniform(lightName + ".position", light.position);
            shaderProgram.updateShaderUniform(lightName + ".color", light.color);
            shaderProgram.updateShaderUniform(lightName + ".intensity", light.intensity);

            if (lightCount >= 4) break;  // Limit to 4 lights for simplicity
        }

        shaderProgram.updateShaderUniform("numLights", lightCount);
    }

} // s3Dive