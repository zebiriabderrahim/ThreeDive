//
// Created by ABDERRAHIM ZEBIRI on 2024-06-30.
//
#pragma once

#ifndef THREEDIVE_ORTHO_CAMERA_H
#define THREEDIVE_ORTHO_CAMERA_H

#include <glm/glm.hpp>
#include "ICamera.h"

namespace s3Dive {

    class OrthoCamera final : public ICamera {

    public:
        OrthoCamera(float left, float right, float bottom, float top);
        ~OrthoCamera() override = default;

        void setPosition(const glm::vec3& position) override { position_ = position; updateView(); }
        void setRotation(float rotation) override { rotation_ = rotation; updateView(); }
        void setViewMatrix(const glm::mat4 &viewMatrix) override { viewMatrix_ = viewMatrix; }
        void setAspectRatio(float aspectRatio) override { }


        [[nodiscard]] const glm::vec3& getPosition() const override { return position_; }
        [[nodiscard]] float getRotation() const override { return rotation_; }
        [[nodiscard]] const glm::mat4& getViewMatrix() const override { return viewMatrix_; }
        [[nodiscard]] const glm::mat4& getProjectionMatrix() const override { return projectionMatrix_; }

    protected:
        void updateView() override;

    private:
        glm::mat4 viewMatrix_{1.0f};
        glm::mat4 projectionMatrix_{1.0f};
        glm::vec3 position_{0.0f};
        float rotation_{0.0f};
    };

} // s3Dive

#endif //THREEDIVE_ORTHO_CAMERA_H
