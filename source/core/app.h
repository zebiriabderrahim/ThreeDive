#ifndef THREEDIVE_APP_H
#define THREEDIVE_APP_H

#include "../camera/camera_controller.h"
#include "../platform/openGLRender/gl_shader_program.h"
#include "../platform/openGLRender/gl_vertex_array.h"
#include "../platform/openGLRender/gl_vertex_buffer.h"
#include "../scene/scene.h"
#include "../scene/sceneGridSystem.h"
#include "window.h"
#include "../scene/MeshLoadingSystem.h"
#include "../scene/RenderSystem.h"
#include <memory>
#include <vector>

namespace s3Dive {

    class App {
    public:
        explicit App(const std::shared_ptr<Window>& window);
        ~App();

        bool initialize();
        void run();

    private:
        std::shared_ptr<Window> window_;
        CameraController cameraController_{};
        GLShaderProgram gridShader_;
        GLShaderProgram defaultShaderProgram_;
        GLVertexArray vao_;
        std::shared_ptr<GLVertexBuffer> vbo_;
        std::vector<float> gridVertices_;

        Scene scene_;
        SceneGridSystem systems_;
        RenderSystem defaultRenderSystem;
        ModelLoadingSystem meshLoadingSystem_;


        void onRender();
        void initializeGrid();
        void initializeSystems();

        void createDefaultLights();
    };

} // namespace s3Dive

#endif //THREEDIVE_APP_H
