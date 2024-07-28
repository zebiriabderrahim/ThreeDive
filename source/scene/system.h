//
// Created by ABDERRAHIM ZEBIRI on 2024-07-27.
//

#ifndef THREEDIVE_SYSTEM_H
#define THREEDIVE_SYSTEM_H

#include "scene.h"
#include "../platform/openGLRender/gl_shader_program.h"
#include "../camera/camera_controller.h"

namespace s3Dive {

    class System {
    public:
        virtual ~System() = default;
        virtual void update(Scene& scene, float deltaTime) {};
        virtual void render(Scene& scene, GLShaderProgram& shaderProgram, const CameraController& cameraController) {};
    };

} // s3Dive

#endif //THREEDIVE_SYSTEM_H
