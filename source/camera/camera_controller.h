//
// Created by ABDERRAHIM ZEBIRI on 2024-07-17.
//


#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include <variant>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../events/event.h"
#include "camera.h"
#include "perspective_camera.h"
#include "orthographic_camera.h"

namespace s3Dive {

    struct CameraSettings {
        float fieldOfView = 45.0f;
        float nearPlane = 0.1f;
        float farPlane = 1000.0f;
        float minDistance = 1.0f;
        float maxDistance = 100.0f;
        float orbitSpeed = 0.007f;
        float zoomSpeed = 0.1f;
        float panSpeed = 0.1f;
        glm::vec3 initialPosition = glm::vec3(40.0f, 40.0f, 40.0f);
        glm::vec3 targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 upVector = glm::vec3(0.0f, 0.0f, 1.0f);
    };

    class CameraController {
    public:
        explicit CameraController(const CameraSettings& settings = CameraSettings{});

        void update();
        void onEvent(const EventVariant& event);

        void setViewportSize(unsigned int width, unsigned int height);

        [[nodiscard]] const Camera& getCamera() const;
        [[nodiscard]] Camera& getCamera();
        [[nodiscard]] float getDetailVisibility1() const { return detailVisibility1_; }
        [[nodiscard]] float getDetailVisibility2() const { return detailVisibility2_; }

        template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
        template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;


    private:
        using CameraVariant = std::variant<PerspectiveCamera, OrthographicCamera>;
        CameraVariant cameraVariant_;
        CameraSettings settings_;

        glm::vec3 position_{};
        glm::vec2 lastMousePos_{};
        float distance_{};

        bool isOrbiting_ = false;
        bool isPanning_ = false;
        float aspectRatio_ = 1.0f;

        float detailVisibility1_ = 1.0f;
        float detailVisibility2_ = 1.0f;
        static constexpr float kDetailVisibilityNear1 = 20.0f;
        static constexpr float kDetailVisibilityFar1 = 40.0f;
        static constexpr float kDetailVisibilityNear2 = 10.0f;
        static constexpr float kDetailVisibilityFar2 = 20.0f;

        void handleMouseOrbit(const glm::vec2& mouseDelta);
        void handleMouseZoom(float zoomDelta);
        void handleMousePan(const glm::vec2 &panDelta);

        void updateCameraMatrices();
        void initializeView();

        void handleMouseMoved(const MouseMovedEvent& e);
        void handleMouseButtonPressed(const MouseButtonPressedEvent& e);
        void handleMouseButtonReleased(const MouseButtonReleasedEvent& e);
        void handleMouseScrolled(const MouseScrolledEvent& e);
        void handleWindowResize(const WindowResizeEvent& e);
        void handleKeyPressed(const KeyPressedEvent& e);

        void updatePerspectiveProjection(PerspectiveCamera &camera) const;
        void updateOrthographicProjection(OrthographicCamera &camera) const;
    };

} // namespace s3Dive

#endif //CAMERA_CONTROLLER_H