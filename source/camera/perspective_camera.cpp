#include <glm/gtc/matrix_transform.hpp>
#include "perspective_camera.h"

namespace s3Dive {

    PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearPlane, float farPlane)
            : fov_(fov), aspectRatio_(aspectRatio), nearPlane_(nearPlane), farPlane_(farPlane) {
        updateProjectionMatrix();
        updateViewMatrix();
    }

    void PerspectiveCamera::updateViewMatrix() {
        viewMatrix_ = glm::lookAt(position_,target_,up_);
    }

    void PerspectiveCamera::updateProjectionMatrix() {
        projectionMatrix_ = glm::perspective(glm::radians(fov_),
                                             aspectRatio_,
                                             nearPlane_,
                                             farPlane_);
    }

} // s3Dive