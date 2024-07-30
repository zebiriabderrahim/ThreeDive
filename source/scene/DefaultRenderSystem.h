//
// Created by ABDERRAHIM ZEBIRI on 2024-07-29.
//

#ifndef THREEDIVE_DEFAULTRENDERSYSTEM_H
#define THREEDIVE_DEFAULTRENDERSYSTEM_H

#include "system.h"
#include "components.h"

namespace s3Dive {

        class DefaultRenderSystem : public System {
        public:
            void update(Scene& scene, float deltaTime) override;
            void render(Scene& scene, GLShaderProgram& shaderProgram);
        };

} // s3Dive

#endif //THREEDIVE_DEFAULTRENDERSYSTEM_H
