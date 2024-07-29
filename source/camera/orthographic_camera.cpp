#include <glm/gtc/matrix_transform.hpp>
#include "orthographic_camera.h"

namespace s3Dive {

    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
            : left_(left), right_(right), bottom_(bottom), top_(top), nearPlane_(DEFAULT_NEAR_PLANE),
              farPlane_(DEFAULT_FAR_PLANE) {
        updateProjectionMatrix();
    }


    void OrthographicCamera::updateProjectionMatrix() {
        projectionMatrix_ = glm::ortho(left_, right_, bottom_, top_, nearPlane_, farPlane_);
    }

} // s3Dive