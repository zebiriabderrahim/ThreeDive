//
// Created by ABDERRAHIM ZEBIRI on 2024-07-01.
//

#ifndef THREEDIVE_ICAMERA_H
#define THREEDIVE_ICAMERA_H

#include <glm/glm.hpp>

namespace s3Dive {

    class ICamera
    {
    public:
        virtual ~ICamera() = default;

        virtual void setPosition(const glm::vec3& position) = 0;
        virtual void setRotation(float rotation) = 0;
        virtual const glm::mat4& getViewMatrix() const = 0;
        virtual const glm::mat4& getProjectionMatrix() const = 0;
        virtual const glm::vec3& getPosition() const = 0;
        virtual float getRotation() const = 0;

    protected:
        virtual void updateView() = 0;
    };


} // s3Dive

#endif //THREEDIVE_ICAMERA_H
