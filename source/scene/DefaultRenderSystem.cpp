#include "DefaultRenderSystem.h"
#include "components.h"
#include "../renderer/RenderCommand.h"

namespace s3Dive {

    void DefaultRenderSystem::update(Scene& scene, float deltaTime) {
        // Update logic if needed
    }

    void DefaultRenderSystem::render(Scene& scene, GLShaderProgram& shaderProgram) {
        auto view = scene.view<MeshComponent>();
        for (auto entity : view) {
            const auto& mesh = view.get<MeshComponent>(entity);
            if (!mesh.isInitialized || !mesh.vertexArray) {
                continue;
            }

            shaderProgram.use();

            // Set material properties
            shaderProgram.updateShaderUniform("material.ambient", mesh.ambientColor);
            shaderProgram.updateShaderUniform("material.diffuse", mesh.diffuseColor);
            shaderProgram.updateShaderUniform("material.specular", mesh.specularColor);
            shaderProgram.updateShaderUniform("material.shininess", mesh.shininess);


            shaderProgram.updateShaderUniform("useTexture", mesh.texture != nullptr);
            if (mesh.texture) {
                mesh.texture->bind();
            }

            mesh.vertexArray->bind();
            RenderCommand::drawIndexed(*mesh.vertexArray, mesh.indices.size());
            mesh.vertexArray->unbind();
            shaderProgram.unuse();
        }
    }

} // s3Dive