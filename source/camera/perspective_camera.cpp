#include <glm/gtc/matrix_transform.hpp>
#include "perspective_camera.h"

namespace s3Dive {

    PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearPlane, float farPlane)
            : fov_(fov), aspectRatio_(aspectRatio), nearPlane_(nearPlane), farPlane_(farPlane) {
        updateProjectionMatrix();
    }

    void PerspectiveCamera::updateProjectionMatrix() {
        projectionMatrix_ = glm::perspective(glm::radians(fov_),
                                             aspectRatio_,
                                             nearPlane_,
                                             farPlane_);
    }

} // s3Dive