//
// Created by ABDERRAHIM ZEBIRI on 2024-07-01.
//
#pragma once

#ifndef THREEDIVE_PERSPECTIVE_CAMERA_H
#define THREEDIVE_PERSPECTIVE_CAMERA_H


#include "ICamera.h"

namespace s3Dive {

    class PerspectiveCamera final : public ICamera {
    public:
        PerspectiveCamera(float fov, float aspectRatio, float nearPlane, float farPlane);
        ~PerspectiveCamera() override = default;

        void setPosition(const glm::vec3& position) override { position_ = position; updateView(); }
        void setRotation(float rotation) override { rotation_ = rotation; updateView(); }
        void setViewMatrix(const glm::mat4 &viewMatrix) override { viewMatrix_ = viewMatrix;}
        void setAspectRatio(float aspectRatio) override { aspectRatio_ = aspectRatio; updateView(); }
        void setProjectionMatrix(const glm::mat4 &projectionMatrix) override { projectionMatrix_ = projectionMatrix; }

        [[nodiscard]] const glm::mat4& getViewMatrix() const override { return viewMatrix_; }
        [[nodiscard]] const glm::mat4& getProjectionMatrix() const override { return projectionMatrix_; }
        [[nodiscard]] const glm::vec3& getPosition() const override { return position_; }
        [[nodiscard]] float getRotation() const override { return rotation_; }

    protected:
        void updateView() override;

    private:
        glm::vec3 position_{0.0f};
        float rotation_{0.0f};
        glm::mat4 viewMatrix_{1.0f};
        glm::mat4 projectionMatrix_{1.0f};
        float fov_;
        float aspectRatio_;
        float nearPlane_;
        float farPlane_;
    };

} // s3Dive

#endif //THREEDIVE_PERSPECTIVE_CAMERA_H
