#include <glm/gtc/matrix_transform.hpp>
#include "ortho_camera.h"

namespace s3Dive {

    OrthoCamera::OrthoCamera(float left, float right, float bottom, float top):
    projectionMatrix_(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)){}

    void OrthoCamera::updateView()
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position_) *
                              glm::rotate(glm::mat4(1.0f),
                                          glm::radians(rotation_),
                                          glm::vec3(0, 0, 1)
                                          );

        viewMatrix_ = glm::inverse(transform);
    }

} // s3Dive