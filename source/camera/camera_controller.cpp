#include "camera_controller.h"
#include <algorithm>
#include <spdlog/spdlog.h>

namespace s3Dive {

    CameraController::CameraController(const CameraSettings& settings): settings_(settings)
    {
        cameraVariant_.emplace<PerspectiveCamera>(PerspectiveCamera(45.0f,
                                                                    aspectRatio_,
                                                                    settings_.nearPlane,
                                                                    settings_.farPlane));
        initializeView();
        updateCameraMatrices();
    }

    void CameraController::initializeView()
    {
        float radius = glm::length(settings_.initialPosition - settings_.targetPosition);
        float theta = glm::pi<float>() / 4.0f;
        float phi = glm::pi<float>() / 4.0f;

        position_.x = settings_.targetPosition.x + radius * std::sin(phi) * std::cos(theta);
        position_.y = settings_.targetPosition.y + radius * std::sin(phi) * std::sin(theta);
        position_.z = settings_.targetPosition.z + radius * std::cos(phi);

        distance_ = radius;
    }

    void CameraController::update()
    {
        float distanceToTarget = glm::distance(position_, settings_.targetPosition);
        detailVisibility1_ = 1.0f - glm::clamp((distanceToTarget - kDetailVisibilityNear1) / (kDetailVisibilityFar1 - kDetailVisibilityNear1), 0.0f, 1.0f);
        detailVisibility2_ = 1.0f - glm::clamp((distanceToTarget - kDetailVisibilityNear2) / (kDetailVisibilityFar2 - kDetailVisibilityNear2), 0.0f, 1.0f);

        updateCameraMatrices();
    }

    void CameraController::onEvent(const EventVariant& event)
    {
        std::visit(overloaded{
                [this](const MouseMovedEvent& e) { handleMouseMoved(e); },
                [this](const MouseButtonPressedEvent& e) { handleMouseButtonPressed(e); },
                [this](const MouseButtonReleasedEvent& e) { handleMouseButtonReleased(e); },
                [this](const MouseScrolledEvent& e) { handleMouseScrolled(e); },
                [this](const WindowResizeEvent& e) { handleWindowResize(e); },
                [this](const KeyPressedEvent& e) { handleKeyPressed(e); },
                [](const auto&) {
                    spdlog::warn("Unhandled event type");
                }
        }, event);
    }

    void CameraController::handleMouseMoved(const MouseMovedEvent& e)
    {
        if (isOrbiting_) {
            glm::vec2 currentPos(e.x, e.y);
            glm::vec2 delta = currentPos - lastMousePos_;
            handleMouseOrbit(delta);
            lastMousePos_ = currentPos;
        }
    }

    void CameraController::handleMouseButtonPressed(const MouseButtonPressedEvent& e)
    {
        if (e.button == MouseCode::ButtonLeft) {
            isOrbiting_ = true;
            lastMousePos_ = glm::vec2(e.x, e.y);
        }
    }

    void CameraController::handleMouseButtonReleased(const MouseButtonReleasedEvent& e)
    {
        if (e.button == MouseCode::ButtonLeft) {
            isOrbiting_ = false;
        }
    }

    void CameraController::handleMouseScrolled(const MouseScrolledEvent& e)
    {
        handleMouseZoom(glm::abs(e.yOffset) >= glm::abs(e.xOffset) ? e.yOffset : e.xOffset);
    }

    void CameraController::handleWindowResize(const WindowResizeEvent& e)
    {
        setViewportSize(e.width, e.height);
    }

    void CameraController::handleKeyPressed(const KeyPressedEvent& e)
    {
        if (e.keyCode == Key::KeyCode::P && !std::holds_alternative<PerspectiveCamera>(cameraVariant_)) {
            spdlog::info("Switching to perspective camera");
            cameraVariant_.emplace<PerspectiveCamera>(45.0f, aspectRatio_, settings_.nearPlane, settings_.farPlane);
            updateCameraMatrices();
        } else if (e.keyCode == Key::KeyCode::O && !std::holds_alternative<OrthographicCamera>(cameraVariant_)) {
            spdlog::info("Switching to orthographic camera");
            float orthoSize = distance_ * 0.5f;
            float orthoWidth = orthoSize * aspectRatio_;
            float orthoHeight = orthoSize;
            cameraVariant_.emplace<OrthographicCamera>(-orthoWidth, orthoWidth, -orthoHeight, orthoHeight);
            updateCameraMatrices();
        }
    }

    void CameraController::setViewportSize(unsigned int width, unsigned int height)
    {
        aspectRatio_ = static_cast<float>(width) / static_cast<float>(height);

        auto updateProjectionVisitor = [this](auto& camera) {
            if constexpr (std::is_same_v<std::decay_t<decltype(camera)>, PerspectiveCamera>) {
                camera.setAspectRatio(aspectRatio_);
            } else if constexpr (std::is_same_v<std::decay_t<decltype(camera)>, OrthographicCamera>) {
                updateOrthographicProjection(camera);
            }
        };

        std::visit(updateProjectionVisitor, cameraVariant_);
        updateCameraMatrices();
    }

    void CameraController::handleMouseOrbit(const glm::vec2& mouseDelta)
    {
        float radius = glm::length(position_ - settings_.targetPosition);
        float theta = std::atan2(position_.y - settings_.targetPosition.y, position_.x - settings_.targetPosition.x);
        float phi = std::acos((position_.z - settings_.targetPosition.z) / radius);

        theta -= mouseDelta.x * settings_.orbitSpeed;
        phi -= mouseDelta.y * settings_.orbitSpeed;

        phi = glm::clamp(phi, 0.01f, glm::pi<float>() - 0.01f);

        position_.x = settings_.targetPosition.x + radius * std::sin(phi) * std::cos(theta);
        position_.y = settings_.targetPosition.y + radius * std::sin(phi) * std::sin(theta);
        position_.z = settings_.targetPosition.z + radius * std::cos(phi);
    }

    void CameraController::handleMouseZoom(float zoomDelta)
    {
        distance_ -= zoomDelta * settings_.zoomSpeed;
        distance_ = std::clamp(distance_, settings_.minDistance, settings_.maxDistance);

        glm::vec3 direction = glm::normalize(settings_.targetPosition - position_);
        position_ = settings_.targetPosition - direction * distance_;

        auto updateProjectionVisitor = [this](auto& camera) {
            if constexpr (std::is_same_v<std::decay_t<decltype(camera)>, OrthographicCamera>) {
                updateOrthographicProjection(camera);
            }
        };
        std::visit(updateProjectionVisitor, cameraVariant_);

        updateCameraMatrices();
    }

    void CameraController::updateCameraMatrices()
    {
        glm::mat4 viewMatrix = glm::lookAt(position_, settings_.targetPosition, settings_.upVector);
        auto updateProjectionVisitor = [this, &viewMatrix](auto& camera) {
            if constexpr (std::is_same_v<std::decay_t<decltype(camera)>, PerspectiveCamera>) {
                updatePerspectiveProjection(camera);
            } else if constexpr (std::is_same_v<std::decay_t<decltype(camera)>, OrthographicCamera>) {
                updateOrthographicProjection(camera);
            }
            camera.setViewMatrix(viewMatrix);
        };

        std::visit(updateProjectionVisitor, cameraVariant_);
    }

    const Camera& CameraController::getCamera() const
    {
        return std::visit([](const auto& camera) -> const Camera& { return camera; }, cameraVariant_);
    }

    Camera& CameraController::getCamera()
    {
        return std::visit([](auto& camera) -> Camera& { return camera; }, cameraVariant_);
    }

    void CameraController::updatePerspectiveProjection(PerspectiveCamera& camera) const {
        glm::mat4 projectionMatrix = glm::perspective(glm::radians(settings_.fieldOfView), aspectRatio_, settings_.nearPlane, settings_.farPlane);
        camera.setProjectionMatrix(projectionMatrix);
    }

    void CameraController::updateOrthographicProjection(OrthographicCamera& camera) const {
        float orthoSize = distance_ * 0.5f;
        float orthoWidth = orthoSize * aspectRatio_;
        float orthoHeight = orthoSize;
        glm::mat4 projectionMatrix = glm::ortho(-orthoWidth, orthoWidth, -orthoHeight, orthoHeight, settings_.nearPlane, settings_.farPlane);
        camera.setProjectionMatrix(projectionMatrix);
    }

} // namespace s3Dive