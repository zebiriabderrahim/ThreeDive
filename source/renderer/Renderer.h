//
// Created by ABDERRAHIM ZEBIRI on 2024-07-15.
//

#ifndef THREEDIVE_RENDERER_H
#define THREEDIVE_RENDERER_H

#include <memory>

#include "../platform/openGLRender/gl_vertex_array.h"
#include "RenderCommand.h"

namespace s3Dive {

    class Renderer {

    public:
        static void init();

        static void beginScene();
        static void endScene();

        static void submit(const std::shared_ptr<GLVertexArray> &vertexArray);

        static void shutdown();

    };

} // s3Dive

#endif //THREEDIVE_RENDERER_H
