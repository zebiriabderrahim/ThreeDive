//
// Created by ABDERRAHIM ZEBIRI on 2024-07-21.
//

#ifndef THREEDIVE_SCENEGRIDSYSTEM_H
#define THREEDIVE_SCENEGRIDSYSTEM_H

#include <memory>
#include <vector>

#include "../platform/openGLRender/gl_vertex_array.h"
#include "../platform/openGLRender/gl_vertex_buffer.h"
#include "../platform/openGLRender/gl_shader_program.h"
#include "../camera/camera_controller.h"
#include "components.h"

namespace s3Dive {

    class SceneGridSystem {
    public:
        SceneGridSystem() = default;
        ~SceneGridSystem() = default;
        void render(SceneGridComponent &grid, GLShaderProgram& shaderProgram, const CameraController &cameraController);

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
    };



} // s3Dive

#endif //THREEDIVE_SCENEGRIDSYSTEM_H
