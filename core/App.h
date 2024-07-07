//
// Created by ABDERRAHIM ZEBIRI on 2024-07-06.
//
#pragma once

#ifndef THREEDIVE_APP_H
#define THREEDIVE_APP_H



#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "event.h"
#include "../renderer/ICamera.h"
#include "../renderer/perspective_camera.h"
#include "../renderer/CameraController.h"
#include "../openGLRender/gl_shader_program.h"
#include "../openGLRender/gl_vertex_array.h"
#include "../openGLRender/gl_vertex_buffer.h"
#include "../openGLRender/gl_vertex_buffer_layout.h"

namespace s3Dive {

        class TestApplication {
        public:
            TestApplication(GLFWwindow* window);
            TestApplication(const TestApplication&) = delete;
            TestApplication& operator=(const TestApplication&) = delete;
            TestApplication(TestApplication&&) = default;
            TestApplication& operator=(TestApplication&&) = default;
            ~TestApplication();

            [[nodiscard]] bool initialize();
            void run();
            void onUpdate(float deltaTime);
            void onRender();

        private:
            GLFWwindow* window_;
            std::unique_ptr<ICamera> camera_;
            std::unique_ptr<CameraController> cameraController_;
            GLShaderProgram triangleShader_;
            GLVertexArray vao_;
            std::shared_ptr<GLVertexBuffer> vbo_;

            EventQueue eventQueue_;
            EventDispatcher eventDispatcher_;
            const unsigned int SCR_WIDTH = 800;
            const unsigned int SCR_HEIGHT = 600;

            std::vector<float> gridVertices_;

            void initializeCamera();
            void initializeEventSystem();
            void initializeGrid();
            void processInput(float deltaTime);

            std::vector<float> generateDetailedGridVertices(float size, float step, float extensionSize, int fadeSteps);

            static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
            static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
            static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
            static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        };

} // namespace s3Dive



#endif //THREEDIVE_APP_H
