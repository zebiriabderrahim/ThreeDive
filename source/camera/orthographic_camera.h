//
// Created by ABDERRAHIM ZEBIRI on 2024-06-30.
//

#ifndef THREEDIVE_ORTHOGRAPHIC_CAMERA_H
#define THREEDIVE_ORTHOGRAPHIC_CAMERA_H

#include "camera.h"

namespace s3Dive {

    class OrthographicCamera final : public Camera {

    public:
        OrthographicCamera(float left, float right, float bottom, float top);
        ~OrthographicCamera() override = default;

        void setViewMatrix(const glm::mat4 &viewMatrix) override { viewMatrix_ = viewMatrix; }
        void setProjectionMatrix(const glm::mat4 &projectionMatrix) override { projectionMatrix_ = projectionMatrix; }

        [[nodiscard]] const glm::mat4 &getViewMatrix() const override { return viewMatrix_; }
        [[nodiscard]] const glm::mat4 &getProjectionMatrix() const override { return projectionMatrix_; }

        void updateProjectionMatrix() override;

        static constexpr float DEFAULT_NEAR_PLANE = 0.1f;
        static constexpr float DEFAULT_FAR_PLANE = 100.0f;

    private:
        glm::mat4 viewMatrix_{1.0f};
        glm::mat4 projectionMatrix_{1.0f};


        float left_;
        float right_;
        float bottom_;
        float top_;
        float nearPlane_{-1.0f};
        float farPlane_{1.0f};
    };

} // s3Dive

#endif //THREEDIVE_ORTHOGRAPHIC_CAMERA_H
