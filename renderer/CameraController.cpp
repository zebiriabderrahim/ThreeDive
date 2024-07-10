#include "CameraController.h"
#include <algorithm>
#include "../core/Event.h"


namespace s3Dive {

    CameraController::CameraController(std::unique_ptr<ICamera> camera, const CameraSettings& settings)
            : camera_(std::move(camera)), settings_(settings)
    {
        initializeView();
        updateCameraMatrices();
    }

    void CameraController::initializeView()
    {
        // Set initial spherical coordinates
        float radius = glm::length(settings_.initialPosition - settings_.targetPosition);
        float theta = glm::pi<float>() / 4.0f; // 45 degrees
        float phi = glm::pi<float>() / 4.0f; // 45 degrees

        // Convert to Cartesian coordinates
        position_.x = settings_.targetPosition.x + radius * std::sin(phi) * std::cos(theta);
        position_.y = settings_.targetPosition.y + radius * std::sin(phi) * std::sin(theta);
        position_.z = settings_.targetPosition.z + radius * std::cos(phi);

        // Set initial rotation
        distance_ = radius;
    }


    void CameraController::update()
    {
        // Calculate detail visibility
        float distanceToTarget = glm::distance(position_, settings_.targetPosition);
        detailVisibility1_ = 1.0f - glm::clamp((distanceToTarget - kDetailVisibilityNear1) / (kDetailVisibilityFar1 - kDetailVisibilityNear1), 0.0f, 1.0f);
        detailVisibility2_ = 1.0f - glm::clamp((distanceToTarget - kDetailVisibilityNear2) / (kDetailVisibilityFar2 - kDetailVisibilityNear2), 0.0f, 1.0f);

        updateCameraMatrices();
    }

    template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
    template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

    void CameraController::onEvent(const EventVariant& event)
    {
        auto handleMouseMoved = [this](const MouseMovedEvent& e) {
            if (isOrbiting_) {
                glm::vec2 currentPos(e.x, e.y);
                glm::vec2 delta = currentPos - lastMousePos_;
                handleMouseOrbit(delta);
                lastMousePos_ = currentPos;
            }
        };

        auto handleMouseButtonPressed = [this](const MouseButtonPressedEvent& e) {
            if (e.button == MouseCode::ButtonLeft) {
                isOrbiting_ = true;
                lastMousePos_ = glm::vec2(e.x, e.y);
            }
        };

        auto handleMouseButtonReleased = [this](const MouseButtonReleasedEvent& e) {
            if (e.button == MouseCode::ButtonLeft) {
                isOrbiting_ = false;
            }
        };

        auto handleMouseScrolled = [this](const MouseScrolledEvent& e) {
            handleMouseZoom(e.yOffset);
        };

        auto handleWindowResize = [this](const WindowResizeEvent& e) {
            setViewportSize(e.width, e.height);
        };

        auto handleDefault = [](const auto&) {
            return;
        };

        std::visit(overloaded{
                handleMouseMoved,
                handleMouseButtonPressed,
                handleMouseButtonReleased,
                handleMouseScrolled,
                handleWindowResize,
                handleDefault
        }, event);
    }

    void CameraController::setViewportSize(unsigned int width, unsigned int height)
    {
        aspectRatio_ = static_cast<float>(width) / static_cast<float>(height);
        updateCameraMatrices();
    }

    void CameraController::handleMouseOrbit(const glm::vec2& mouseDelta)
    {
        // Convert current position to spherical coordinates
        float radius = glm::length(position_ - settings_.targetPosition);
        float theta = std::atan2(position_.y - settings_.targetPosition.y, position_.x - settings_.targetPosition.x);
        float phi = std::acos((position_.z - settings_.targetPosition.z) / radius);

        // Update angles based on mouse movement
        theta -= mouseDelta.x * settings_.orbitSpeed;
        phi -= mouseDelta.y * settings_.orbitSpeed;

        // Clamp phi to avoid flipping over the top
        phi = glm::clamp(phi, 0.01f, glm::pi<float>() - 0.01f);

        // Convert back to Cartesian coordinates
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
    }

    void CameraController::updateCameraMatrices()
    {
        glm::mat4 viewMatrix = glm::lookAt(position_, settings_.targetPosition, settings_.upVector);
        glm::mat4 projectionMatrix = glm::perspective(glm::radians(settings_.fieldOfView), aspectRatio_, settings_.nearPlane, settings_.farPlane);

        camera_->setViewMatrix(viewMatrix);
        camera_->setProjectionMatrix(projectionMatrix);
    }

} // namespace s3Dive
