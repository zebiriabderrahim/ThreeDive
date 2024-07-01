//
// Created by ABDERRAHIM ZEBIRI on 2024-06-30.
//

#include "ortho_camera.h"

namespace s3Dive {

    OrthoCamera::OrthoCamera(float left, float right, float bottom, float top)
            : view_(1.0f),
              projection_(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
              position_(0.0f){
    }

    void OrthoCamera::setPosition(const glm::vec3 &position) {
        position_ = position;
        updateView();
    }

    void OrthoCamera::setRotation(float rotation) {
        m_Rotation = rotation;
        updateView();
    }

    const glm::mat4 &OrthoCamera::getView() const {
        return view_;
    }

    const glm::mat4 &OrthoCamera::getProjection() const {
        return projection_;
    }

    const glm::vec3 &OrthoCamera::getPosition() const {
        return position_;
    }

    float OrthoCamera::getRotation() const {
        return m_Rotation;
    }

    void OrthoCamera::updateView() {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position_);
        transform *=glm::rotate(glm::mat4(1.0f),
                                glm::radians(m_Rotation),
                                glm::vec3(0, 0, 1));
        view_ = glm::inverse(transform);
    }

} // s3Dive