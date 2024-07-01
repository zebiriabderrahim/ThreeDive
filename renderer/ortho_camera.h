//
// Created by ABDERRAHIM ZEBIRI on 2024-06-30.
//

#ifndef THREEDIVE_ORTHO_CAMERA_H
#define THREEDIVE_ORTHO_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>


namespace s3Dive {

    class OrthoCamera {

    public:
        OrthoCamera(float left, float right, float bottom, float top);

        void setPosition(const glm::vec3 &position);
        void setRotation(float rotation);

        [[nodiscard]] const glm::mat4 &getView() const;
        [[nodiscard]] const glm::mat4 &getProjection() const;
        [[nodiscard]] const glm::vec3 &getPosition() const;
        [[nodiscard]] float getRotation() const;

    private:
        void updateView();
        glm::mat4 view_{};
        glm::mat4 projection_{};
        glm::vec3 position_{};
        float m_Rotation = 0.0f;
    };

} // s3Dive

#endif //THREEDIVE_ORTHO_CAMERA_H
