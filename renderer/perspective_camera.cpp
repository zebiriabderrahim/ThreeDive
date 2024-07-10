#include <glm/gtc/matrix_transform.hpp>
#include "perspective_camera.h"

namespace s3Dive {

    PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearPlane, float farPlane)
            : fov_(fov)
            , aspectRatio_(aspectRatio)
            , nearPlane_(nearPlane)
            , farPlane_(farPlane)
    {
        projectionMatrix_ = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    }

    void PerspectiveCamera::updateView()
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position_)
                              * glm::rotate(glm::mat4(1.0f), glm::radians(rotation_), glm::vec3(0, 1, 0));

        viewMatrix_ = glm::inverse(transform);
    }

} // s3Dive