//
// Created by ABDERRAHIM ZEBIRI on 2024-07-01.
//
#pragma once

#ifndef THREEDIVE_PERSPECTIVE_CAMERA_H
#define THREEDIVE_PERSPECTIVE_CAMERA_H


#include "camera.h"

namespace s3Dive {

    class PerspectiveCamera final : public Camera {

        static constexpr float DEFAULT_FOV = 45.0f;
        static constexpr float DEFAULT_NEAR_PLANE = 0.1f;
        static constexpr float DEFAULT_FAR_PLANE = 100.0f;

    public:
        explicit PerspectiveCamera(float fov = DEFAULT_FOV, float aspectRatio = 1.0f, float nearPlane =DEFAULT_NEAR_PLANE, float farPlane = DEFAULT_FAR_PLANE);
        ~PerspectiveCamera() override = default;

        void setPosition(const glm::vec3 &position) override { position_ = position; updateViewMatrix();}
        void setViewMatrix(const glm::mat4 &viewMatrix) override { viewMatrix_ = viewMatrix; }
        void setProjectionMatrix(const glm::mat4 &projectionMatrix) override { projectionMatrix_ = projectionMatrix; }
        void setAspectRatio(float aspectRatio) { aspectRatio_ = aspectRatio; updateProjectionMatrix(); }

        [[nodiscard]] const glm::mat4 &getViewMatrix() const override { return viewMatrix_; }
        [[nodiscard]] const glm::mat4 &getProjectionMatrix() const override { return projectionMatrix_; }
        [[nodiscard]] const glm::vec3 &getPosition() const override { return position_; }
        [[nodiscard]] const glm::vec3 &getTarget() const override { return target_; };
        [[nodiscard]] const glm::vec3 &getUp() const override { return up_; };

        void updateViewMatrix() override;
        void updateProjectionMatrix() override;

    private:
        glm::mat4 viewMatrix_{1.0f};
        glm::mat4 projectionMatrix_{1.0f};
        glm::vec3 position_{0.0f};
        glm::vec3 target_{0.0f, 0.0f, -1.0f};
        glm::vec3 up_{0.0f, 1.0f, 0.0f};

        float fov_;
        float aspectRatio_;
        float nearPlane_;
        float farPlane_;
    };

} // s3Dive

#endif //THREEDIVE_PERSPECTIVE_CAMERA_H
