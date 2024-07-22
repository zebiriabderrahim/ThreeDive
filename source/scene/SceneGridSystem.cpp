//
// Created by ABDERRAHIM ZEBIRI on 2024-07-21.
//

#include "SceneGridSystem.h"
#include "../renderer/RenderCommand.h"


namespace s3Dive {
    SceneGridSystem::SceneGridSystem(SceneGridComponent &grid, GLShaderProgram& shaderProgram)
            : shaderProgram_(shaderProgram) {
        initialize(grid);
    }

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

    void SceneGridSystem::render(SceneGridComponent &grid, const CameraController &cameraController) {
        if (!grid.isInitialized) {
            initialize(grid);
        }


        if (float currentDistanceToTarget = cameraController.getDistanceToTarget();
                shouldRecalculateVisibility(currentDistanceToTarget)) {
            calculateVisibility(currentDistanceToTarget);
        }

        shaderProgram_.use();
        shaderProgram_.updateShaderUniform("detailVisibility1", cachedDetailVisibility1_);
        shaderProgram_.updateShaderUniform("detailVisibility2", cachedDetailVisibility2_);

        RenderCommand::drawLines(vao_, static_cast<GLint>(grid.vertices.size() / 8));

        shaderProgram_.unuse();
    }
} // s3Dive