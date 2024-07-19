//
// Created by ABDERRAHIM ZEBIRI on 2024-07-01.
//
#pragma once

#ifndef THREEDIVE_PERSPECTIVE_CAMERA_H
#define THREEDIVE_PERSPECTIVE_CAMERA_H


#include "camera.h"

namespace s3Dive {

    class PerspectiveCamera final : public Camera {



    public:
        explicit PerspectiveCamera(float fov = DEFAULT_FOV, float aspectRatio = 1.0f, float nearPlane =DEFAULT_NEAR_PLANE, float farPlane = DEFAULT_FAR_PLANE);
        ~PerspectiveCamera() override = default;

        void setViewMatrix(const glm::mat4 &viewMatrix) override { viewMatrix_ = viewMatrix; }
        void setProjectionMatrix(const glm::mat4 &projectionMatrix) override { projectionMatrix_ = projectionMatrix; }

        [[nodiscard]] const glm::mat4 &getViewMatrix() const override { return viewMatrix_; }
        [[nodiscard]] const glm::mat4 &getProjectionMatrix() const override { return projectionMatrix_; }

        void updateProjectionMatrix() override;

        static constexpr float DEFAULT_FOV = 45.0f;
        static constexpr float DEFAULT_NEAR_PLANE = 0.1f;
        static constexpr float DEFAULT_FAR_PLANE = 100.0f;

    private:
        glm::mat4 viewMatrix_{1.0f};
        glm::mat4 projectionMatrix_{1.0f};

        float fov_;
        float aspectRatio_;
        float nearPlane_;
        float farPlane_;
    };

} // s3Dive

#endif //THREEDIVE_PERSPECTIVE_CAMERA_H
