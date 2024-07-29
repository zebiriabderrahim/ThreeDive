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

    void Renderer::submit(const GLVertexArray& vertexArray) {
        RenderCommand::drawIndexed( vertexArray, vertexArray.getIndexBuffer()->getCount());

    }

    void Renderer::shutdown() {

    }

} // s3Dive