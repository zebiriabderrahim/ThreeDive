//
// Created by ABDERRAHIM ZEBIRI on 2024-06-29.
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include "gl_context.h"
#include "../logging/debug_info.h"

namespace s3Dive {
    GLContext::GLContext(GLFWwindow *window) : window_(window) {

    }

    void GLContext::init() const {
        glfwMakeContextCurrent(window_);
        glfwSwapInterval(1); // Enable vsync

        // Initialize OpenGL loader
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            spdlog::error("Failed to initialize OpenGL context");
            return;
        }

        s3Dive::debug::printGLInfo();
    }

    void GLContext::swapBuffers() const {
        glfwSwapBuffers(window_);
    }
} // s3Dive