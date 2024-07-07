//
// Created by ABDERRAHIM ZEBIRI on 2024-07-06.
//

#ifndef THREEDIVE_CAMERACONTROLLER_H
#define THREEDIVE_CAMERACONTROLLER_H

#pragma once

#include "ICamera.h"
#include "../core/event.h"
#include <memory>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

namespace s3Dive {

    enum class ViewMode {
        Perspective,
        Orthographic
    };

    enum class StandardView {
        Top,
        Front,
        Side,
        Isometric
    };

    class CameraController {
    public:
        CameraController(std::unique_ptr<ICamera> camera, GLFWwindow* window);
        ~CameraController() = default;

        CameraController(const CameraController&) = delete;
        CameraController& operator=(const CameraController&) = delete;
        CameraController(CameraController&&) = default;
        CameraController& operator=(CameraController&&) = default;

        void handleEvent(const EventVariant& event);
        void update(float deltaTime);

        void setViewMode(ViewMode mode);
        void setStandardView(StandardView view);

        [[nodiscard]] const ICamera& getCamera() const { return *camera_; }
        [[nodiscard]] ICamera& getCamera() { return *camera_; }

    private:
        float orbitSpeed_ = 0.1f;
        float panSpeed_ = 0.005f;
        float zoomSpeed_ = 5.0f;
        std::unique_ptr<ICamera> camera_;
        GLFWwindow* window_;
        glm::vec3 focalPoint_{0.0f};
        float orbitRadius_{10.0f};
        ViewMode currentViewMode_{ViewMode::Perspective};

        bool isPanning_ = false;
        bool isOrbiting_ = false;
        glm::dvec2 lastMousePos_{0.0, 0.0};

        void processEvent(const MouseMovedEvent& e);
        void processEvent(const MouseScrolledEvent& e);
        void processEvent(const KeyPressedEvent& e);
        void processEvent(const MouseButtonPressedEvent& e);
        void processEvent(const MouseButtonReleasedEvent& e);

        template<typename T>
        void processEvent(const T&) { /* Do nothing for unhandled events */ }

        void orbit(float deltaX, float deltaY);
        void pan(float deltaX, float deltaY);
        void zoom(float delta);
        void updateCameraPosition();
    };

} // namespace s3Dive

#endif //THREEDIVE_CAMERACONTROLLER_H
