//
// Created by ABDERRAHIM ZEBIRI on 2024-07-17.
//

#ifndef THREEDIVE_CAMERA_H
#define THREEDIVE_CAMERA_H

#include <glm/glm.hpp>

namespace s3Dive {

    class Camera {

    public:
        virtual ~Camera() = default;

        virtual void setPosition(const glm::vec3& position) = 0;
        virtual void setViewMatrix(const glm::mat4& viewMatrix) = 0;
        virtual void setProjectionMatrix(const glm::mat4& projectionMatrix) = 0;

        [[nodiscard]] virtual const glm::mat4& getViewMatrix() const = 0;
        [[nodiscard]] virtual const glm::mat4& getProjectionMatrix() const = 0;
        [[nodiscard]] virtual const glm::vec3& getPosition() const = 0;
        [[nodiscard]] virtual const glm::vec3& getTarget() const = 0;
        [[nodiscard]] virtual const glm::vec3& getUp() const = 0;

        virtual void updateViewMatrix() = 0;
        virtual void updateProjectionMatrix() = 0;

    };

} // s3Dive

#endif //THREEDIVE_CAMERA_H
