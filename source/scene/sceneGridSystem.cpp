#include <spdlog/spdlog.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "sceneGridSystem.h"
#include "../renderer/RenderCommand.h"


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

} // namespace s3Dive
