//
// Created by ABDERRAHIM ZEBIRI on 2024-07-06.
//

#ifndef THREEDIVE_APP_H
#define THREEDIVE_APP_H

#include <memory>
#include <vector>

#include "../camera/perspective_camera.h"
#include "../camera/camera_controller.h"
#include "../platform/openGLRender/gl_shader_program.h"
#include "../platform/openGLRender/gl_vertex_array.h"
#include "../platform/openGLRender/gl_vertex_buffer.h"
#include "window.h"
#include "../scene/scene.h"

namespace s3Dive {

    class App {
    public:
         explicit App(const std::shared_ptr<Window>& window);
        ~App();

        App(const App&) = delete;
        App& operator=(const App&) = delete;

        bool initialize();
        void run();

    private:
        SharedPtr<Window> window_;
        UniquePtr<CameraController> cameraController_;
        GLShaderProgram gridShader_;
        GLVertexArray vao_;
        std::shared_ptr<GLVertexBuffer> vbo_;

        std::vector<float> gridVertices_;

        Scene scene_{};

        void onRender();
        void initializeCamera();
        void initializeGrid();
    };

} // namespace s3Dive



#endif //THREEDIVE_APP_H
