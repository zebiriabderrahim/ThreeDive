#ifndef THREEDIVE_SCENE_GRID_SYSTEM_H
#define THREEDIVE_SCENE_GRID_SYSTEM_H

#include <memory>
#include <vector>

#include "components.h"
#include "scene.h"
#include "system.h"
#include "../platform/openGLRender/gl_vertex_array.h"

namespace s3Dive {

    class SceneGridSystem : public System {
    public:
        SceneGridSystem() = default;
        ~SceneGridSystem() override = default;

        void update(Scene& scene, float deltaTime) override;
        void render(Scene& scene, GLShaderProgram& shaderProgram, const CameraController& cameraController) override;

    private:
        GLVertexArray vao_{};

        static constexpr float kDetailVisibilityNear1 = 20.0f;
        static constexpr float kDetailVisibilityFar1 = 40.0f;
        static constexpr float kDetailVisibilityNear2 = 10.0f;
        static constexpr float kDetailVisibilityFar2 = 20.0f;

        // Cached values
        float cachedDistanceToTarget_ = -1.0f;
        float cachedDetailVisibility1_ = 0.0f;
        float cachedDetailVisibility2_ = 0.0f;

        // Threshold for distance change to trigger recalculation
        static constexpr float kDistanceThreshold = 0.1f;

        void initialize(SceneGridComponent& grid);
        [[nodiscard]] bool shouldRecalculateVisibility(float currentDistance) const;
        void calculateVisibility(float distanceToTarget);
        void renderGrid(SceneGridComponent& grid, GLShaderProgram& shaderProgram, const CameraController& cameraController);
    };



} // namespace s3Dive

#endif // THREEDIVE_SCENE_GRID_SYSTEM_H
