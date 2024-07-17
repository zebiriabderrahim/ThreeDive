//
// Created by ABDERRAHIM ZEBIRI on 2024-07-15.
//

#include "Renderer.h"

namespace s3Dive {


    void Renderer::init() {
        RenderCommand::init();
    }

    void Renderer::beginScene() {

    }

    void Renderer::endScene() {

    }

    void Renderer::submit(const std::shared_ptr<GLVertexArray> &vertexArray) {
        RenderCommand::drawIndexed( *vertexArray);

    }

    void Renderer::shutdown() {

    }

} // s3Dive