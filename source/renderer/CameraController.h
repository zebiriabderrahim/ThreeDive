#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "ICamera.h"
#include "../core/event.h"

namespace s3Dive {

    struct CameraSettings {
        float fieldOfView = 45.0f;
        float nearPlane = 0.1f;
        float farPlane = 1000.0f;
        float minDistance = 1.0f;
        float maxDistance = 100.0f;
        float orbitSpeed = 0.005f;
        float zoomSpeed = 0.1f;
        glm::vec3 initialPosition = glm::vec3(40.0f, 40.0f, 40.0f);
        glm::vec3 targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 upVector = glm::vec3(0.0f, 0.0f, 1.0f);
    };

    class CameraController {
    public:
        explicit CameraController(std::unique_ptr<ICamera> camera, const CameraSettings& settings = CameraSettings{});

        void update();
        void onEvent(const EventVariant& event);

        void setViewportSize(unsigned int width, unsigned int height);

        [[nodiscard]] const ICamera& getCamera() const { return *camera_; }
        [[nodiscard]] ICamera& getCamera() { return *camera_; }
        [[nodiscard]] float getDetailVisibility1() const { return detailVisibility1_; }
        [[nodiscard]] float getDetailVisibility2() const { return detailVisibility2_; }

    private:
        std::unique_ptr<ICamera> camera_;
        CameraSettings settings_;

        glm::vec3 position_{};
        float distance_{};

        glm::vec2 lastMousePos_{};
        bool isOrbiting_ = false;
        float aspectRatio_ = 1.0f;

        float detailVisibility1_ = 1.0f;
        float detailVisibility2_ = 1.0f;
        static constexpr float kDetailVisibilityNear1 = 20.0f;
        static constexpr float kDetailVisibilityFar1 = 40.0f;
        static constexpr float kDetailVisibilityNear2 = 10.0f;
        static constexpr float kDetailVisibilityFar2 = 20.0f;

        void handleMouseOrbit(const glm::vec2& mouseDelta);
        void handleMouseZoom(float zoomDelta);
        void updateCameraMatrices();
        void initializeView();
    };

} // namespace s3Dive
