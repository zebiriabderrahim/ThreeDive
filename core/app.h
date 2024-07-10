//
// Created by ABDERRAHIM ZEBIRI on 2024-07-06.
//
#pragma once

#ifndef THREEDIVE_APP_H
#define THREEDIVE_APP_H

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "../renderer/ICamera.h"
#include "../renderer/perspective_camera.h"
#include "../renderer/CameraController.h"
#include "../openGLRender/gl_shader_program.h"
#include "../openGLRender/gl_vertex_array.h"
#include "../openGLRender/gl_vertex_buffer.h"
#include "../openGLRender/gl_vertex_buffer_layout.h"
#include "../openGLRender/gl_renderer.h"
#include "Window.h"

namespace s3Dive {

    class TestApplication {
    public:
         explicit TestApplication(const std::shared_ptr<Window>& window);
        ~TestApplication();

        TestApplication(const TestApplication&) = delete;
        TestApplication& operator=(const TestApplication&) = delete;

        bool initialize();
        void run();

    private:
        static constexpr unsigned int kScreenWidth = 800;
        static constexpr unsigned int kScreenHeight = 600;

        SharedPtr<Window> window_;
        UniquePtr<CameraController> cameraController_;
        GLShaderProgram gridShader_;
        GLRenderer renderer_;
        GLVertexArray vao_;
        std::shared_ptr<GLVertexBuffer> vbo_;

        std::vector<float> gridVertices_;

        void onRender();
        void initializeCamera();
        void initializeEventSystem();
        void initializeGrid();
        void processEvents();

        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
        static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void windowResizeCallback(GLFWwindow* window, int width, int height);
    };

} // namespace s3Dive



#endif //THREEDIVE_APP_H
