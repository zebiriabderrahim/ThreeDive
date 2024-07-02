//
// Created by ABDERRAHIM ZEBIRI on 2024-06-30.
//

#ifndef THREEDIVE_ORTHO_CAMERA_H
#define THREEDIVE_ORTHO_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "ICamera.h"


namespace s3Dive {

    class OrthoCamera: public ICamera{

    public:
        OrthoCamera(float left, float right, float bottom, float top);

        void setPosition(const glm::vec3 &position) override { position_ = position; updateView(); }
        void setRotation(float rotation) override { m_Rotation = rotation; updateView(); }

        [[nodiscard]] const glm::vec3 &getPosition() const override { return position_; }
        [[nodiscard]] float getRotation() const override { return m_Rotation; }
        [[nodiscard]] const glm::mat4 &getViewMatrix() const override { return view_; }
        [[nodiscard]] const glm::mat4 &getProjectionMatrix() const override { return projection_; }

    private:
        void updateView() override;
        glm::mat4 view_{};
        glm::mat4 projection_{};
        glm::vec3 position_{};
        float m_Rotation = 0.0f;
    };

} // s3Dive

#endif //THREEDIVE_ORTHO_CAMERA_H
