//
// Created by ABDERRAHIM ZEBIRI on 2024-07-09.
//
#ifndef THREEDIVE_SCENEGRID_H
#define THREEDIVE_SCENEGRID_H

#include <vector>
#include "../openGLRender/gl_vertex_buffer.h"
#include "../openGLRender/gl_vertex_array.h"
#include "../openGLRender/gl_shader_program.h"
#include "../renderer/CameraController.h"
#include "../core/memory_and_binding.h"

namespace s3Dive {

    class SceneGrid {
    public:
        explicit SceneGrid(int size = 10, float step = 1.0f, float extensionSize = 0.1f, int fadeSteps = 3);
        ~SceneGrid() = default;

        SceneGrid(const SceneGrid&) = delete;
        SceneGrid& operator=(const SceneGrid&) = delete;
        SceneGrid(SceneGrid&&) = delete;
        SceneGrid& operator=(SceneGrid&&) = delete;

        void render(GLShaderProgram& shaderProgram, UniquePtr<CameraController> &cameraController) const;
        bool isReady() const { return isInitialized_; }

        // Getters
        int getSize() const { return size_; }
        float getStep() const { return step_; }
        float getExtensionSize() const { return extensionSize_; }
        int getFadeSteps() const { return fadeSteps_; }

    private:
        std::vector<float> generateGridVertices() const;
        void initializeOpenGLObjects();

        std::shared_ptr<GLVertexBuffer> VBO_;
        GLVertexArray VAO_;

        int size_;
        float step_;
        float extensionSize_;
        int fadeSteps_;
        bool isInitialized_ = false;
        int  vertexCount_;
    };

} // s3dive

#endif //THREEDIVE_SCENEGRID_H
