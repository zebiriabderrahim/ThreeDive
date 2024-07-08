#include "CameraController.h"
#include <algorithm>

namespace s3Dive {

    CameraController::CameraController(std::unique_ptr<ICamera> camera, const InputManager& inputManager, const CameraSettings& settings)
            : camera_(std::move(camera)), inputManager_(inputManager), settings_(settings)
    {
        initializeView();
        updateCameraMatrices();
    }

    void CameraController::initializeView()
    {
        position_ = settings_.initialPosition;
        glm::vec3 direction = glm::normalize(settings_.targetPosition - position_);
        rotation_ = glm::quatLookAt(direction, settings_.upVector);
        distance_ = glm::length(settings_.initialPosition - settings_.targetPosition);
    }

    void CameraController::update(float deltaTime)
    {
        if (inputManager_.isLeftMouseButtonPressed()) {
            if (!isOrbiting_) {
                isOrbiting_ = true;
                lastMousePos_ = inputManager_.getMousePosition();
            } else {
                glm::vec2 currentMousePos = inputManager_.getMousePosition();
                glm::vec2 mouseDelta = currentMousePos - lastMousePos_;
                handleMouseOrbit(mouseDelta);
                lastMousePos_ = currentMousePos;
            }
        } else {
            isOrbiting_ = false;
        }

        float scrollDelta = inputManager_.getScrollDelta();
        if (scrollDelta != 0.0f) {
            handleMouseZoom(scrollDelta);
        }

        // Calculate detail visibility
        float distanceToTarget = glm::distance(position_, settings_.targetPosition);
        detailVisibility1_ = 1.0f - glm::clamp((distanceToTarget - kDetailVisibilityNear1) / (kDetailVisibilityFar1 - kDetailVisibilityNear1), 0.0f, 1.0f);
        detailVisibility2_ = 1.0f - glm::clamp((distanceToTarget - kDetailVisibilityNear2) / (kDetailVisibilityFar2 - kDetailVisibilityNear2), 0.0f, 1.0f);

        updateCameraMatrices();
    }

    void CameraController::setViewportSize(int width, int height)
    {
        aspectRatio_ = static_cast<float>(width) / static_cast<float>(height);
        updateCameraMatrices();
    }

    void CameraController::handleMouseOrbit(const glm::vec2& mouseDelta)
    {
        // Convert mouse delta to rotation angles
        float yawAngle = -mouseDelta.x * settings_.orbitSpeed;
        float pitchAngle = -mouseDelta.y * settings_.orbitSpeed;

        // Create rotation quaternions
        glm::quat yawQuat = glm::angleAxis(yawAngle, settings_.upVector);
        glm::quat pitchQuat = glm::angleAxis(pitchAngle, glm::cross(settings_.upVector, position_ - settings_.targetPosition));

        // Apply rotations
        glm::vec3 rotatedPosition = yawQuat * (position_ - settings_.targetPosition);
        rotatedPosition = pitchQuat * rotatedPosition;
        position_ = rotatedPosition + settings_.targetPosition;

        // Update rotation
        rotation_ = glm::quatLookAt(glm::normalize(settings_.targetPosition - position_), settings_.upVector);
    }

    void CameraController::handleMouseZoom(float zoomDelta)
    {
        distance_ -= zoomDelta * settings_.zoomSpeed;
        distance_ = std::clamp(distance_, settings_.minDistance, settings_.maxDistance);

        glm::vec3 direction = glm::normalize(settings_.targetPosition - position_);
        position_ = settings_.targetPosition - direction * distance_;
    }

    void CameraController::updateCameraMatrices()
    {
        glm::mat4 viewMatrix = glm::lookAt(position_, settings_.targetPosition, settings_.upVector);
        glm::mat4 projectionMatrix = glm::perspective(glm::radians(settings_.fieldOfView), aspectRatio_, settings_.nearPlane, settings_.farPlane);

        camera_->setViewMatrix(viewMatrix);
        camera_->setProjectionMatrix(projectionMatrix);
    }

} // namespace s3Dive
