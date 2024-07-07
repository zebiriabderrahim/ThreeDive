#include "CameraController.h"
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <algorithm>
#include <iostream>

namespace s3Dive {

    CameraController::CameraController(std::unique_ptr<ICamera> camera, GLFWwindow* window)
            : camera_(std::move(camera)), window_(window), focalPoint_(0.0f, 0.0f, 0.0f)
    {
        updateCameraPosition();
    }

    void CameraController::handleEvent(const EventVariant& event)
    {
        std::visit([this](const auto& e) {
            std::cout << "Handling event: " << typeid(e).name() << std::endl;
            this->processEvent(e);
        }, event);
    }

    void CameraController::update(float deltaTime)
    {
        if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
            zoom(-deltaTime * zoomSpeed_);
        }
        if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {
            zoom(deltaTime * zoomSpeed_);
        }
    }

    void CameraController::setViewMode(ViewMode mode)
    {
        currentViewMode_ = mode;
        // Implement switching between perspective and orthographic cameras
    }

    void CameraController::setStandardView(StandardView view)
    {
        glm::vec3 newPosition;
        float newRotation;

        switch (view) {
            case StandardView::Top:
                newPosition = glm::vec3(0.0f, orbitRadius_, 0.0f);
                newRotation = -90.0f;
                break;
            case StandardView::Front:
                newPosition = glm::vec3(0.0f, 0.0f, orbitRadius_);
                newRotation = 0.0f;
                break;
            case StandardView::Side:
                newPosition = glm::vec3(orbitRadius_, 0.0f, 0.0f);
                newRotation = -90.0f;
                break;
            case StandardView::Isometric:
                newPosition = glm::vec3(orbitRadius_ * 0.707f, orbitRadius_ * 0.707f, orbitRadius_ * 0.707f);
                newRotation = -45.0f;
                break;
        }

        camera_->setPosition(newPosition);
        camera_->setRotation(newRotation);
        updateCameraPosition();
    }

    void CameraController::orbit(float deltaX, float deltaY)
    {
        glm::vec3 toCamera = camera_->getPosition() - focalPoint_;

        float yaw = deltaX * orbitSpeed_;
        float pitch = deltaY * orbitSpeed_;

        // Rotate around world up axis for yaw
        toCamera = glm::rotate(toCamera, glm::radians(-yaw), glm::vec3(0.0f, 1.0f, 0.0f));

        // Rotate around local right axis for pitch
        glm::vec3 right = glm::cross(toCamera, glm::vec3(0.0f, 1.0f, 0.0f));
        toCamera = glm::rotate(toCamera, glm::radians(-pitch), right);

        camera_->setPosition(focalPoint_ + toCamera);
        updateCameraPosition();
    }

    void CameraController::pan(float deltaX, float deltaY)
    {
        glm::vec3 forward = glm::normalize(focalPoint_ - camera_->getPosition());
        glm::vec3 right = glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec3 up = glm::cross(right, forward);

        glm::vec3 pan = right * (-deltaX * panSpeed_) + up * (deltaY * panSpeed_);

        camera_->setPosition(camera_->getPosition() + pan);
        focalPoint_ += pan;
        updateCameraPosition();
    }

    void CameraController::zoom(float delta)
    {
        orbitRadius_ = std::max(0.1f, orbitRadius_ - delta * zoomSpeed_);
        updateCameraPosition();
    }

    void CameraController::processEvent(const MouseMovedEvent& e)
    {
        if (isOrbiting_ || isPanning_) {
            float deltaX = e.x - static_cast<float>(lastMousePos_.x);
            float deltaY = e.y - static_cast<float >(lastMousePos_.y);

            if (isPanning_) {
                pan(deltaX, deltaY);
            } else if (isOrbiting_) {
                orbit(deltaX, deltaY);
            }
        }

        lastMousePos_ = {e.x, e.y};
    }

    void CameraController::processEvent(const MouseScrolledEvent& e)
    {
        zoom(e.yOffset);
    }

    void CameraController::processEvent(const KeyPressedEvent& e)
    {
        switch (e.keyCode) {
            case Key::KeyCode::T: setStandardView(StandardView::Top); break;
            case Key::KeyCode::F: setStandardView(StandardView::Front); break;
            case Key::KeyCode::S: setStandardView(StandardView::Side); break;
            case Key::KeyCode::I: setStandardView(StandardView::Isometric); break;
        }
    }

    void CameraController::processEvent(const MouseButtonPressedEvent& e)
    {
        if (e.button == MouseCode::ButtonMiddle) {
            isPanning_ = true;
        } else if (e.button == MouseCode::ButtonLeft) {
            isOrbiting_ = true;
        }
        glfwGetCursorPos(window_, &lastMousePos_.x, &lastMousePos_.y);
    }

    void CameraController::processEvent(const MouseButtonReleasedEvent& e)
    {
        if (e.button == MouseCode::ButtonMiddle) {
            isPanning_ = false;
        } else if (e.button == MouseCode::ButtonLeft) {
            isOrbiting_ = false;
        }
    }

    void CameraController::updateCameraPosition()
    {
        glm::vec3 direction = glm::normalize(camera_->getPosition() - focalPoint_);
        camera_->setPosition(focalPoint_ + direction * orbitRadius_);

        // Debug output
        glm::vec3 pos = camera_->getPosition();
        std::cout << "Camera position: (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
    }

} // namespace s3Dive
