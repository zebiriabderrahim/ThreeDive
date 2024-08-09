//
// Created by ABDERRAHIM ZEBIRI on 2024-07-29.
//

#ifndef THREEDIVE_RENDERSYSTEM_H
#define THREEDIVE_RENDERSYSTEM_H

#include "system.h"
#include "components.h"

namespace s3Dive {

    class RenderSystem : public System {
    public:
        void render(Scene& scene, GLShaderProgram& shaderProgram,  const CameraController& cameraController) override;

    private:
        void setupLights(Scene& scene, GLShaderProgram& shaderProgram) const;
    };

} // s3Dive

#endif //THREEDIVE_RENDERSYSTEM_H
