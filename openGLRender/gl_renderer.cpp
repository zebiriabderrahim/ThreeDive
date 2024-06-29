//
// Created by ABDERRAHIM ZEBIRI on 2024-06-29.
//

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"

#include "gl_renderer.h"


namespace s3Dive {
    void GLRenderer::init() const {
        if (!glfwInit()) {
            spdlog::error("Failed to initialize GLFW");
            return;
        }

        // Decide GL+GLSL versions
#if __APPLE__
        // GL 3.2 + GLSL 150
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
        // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    }

    void GLRenderer::clear() const {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void GLRenderer::setviewport(int x, int y, int width, int height) const {
        glViewport(x, y, width, height);
    }

    void GLRenderer::setClearColor(float r, float g, float b, float a) const {
        glClearColor(r, g, b, a);
    }

    void GLRenderer::setlineWidth(float width) const {
        glLineWidth(width);
    }

    void GLRenderer::drawIndexed(const GLVertexArray &vao, GLuint indexCount) const {
        vao.bind();
        auto count = indexCount ? indexCount : vao.getIndexBuffer()->getCount();
        glDrawElements(GL_TRIANGLES, static_cast<GLint>(count), GL_UNSIGNED_INT, nullptr);
    }

    void GLRenderer::drawlines(const GLVertexArray &vao, GLuint indexCount) const {
        vao.bind();
        glDrawArrays(GL_LINES, 0, static_cast<GLint>(indexCount));
    }
} // s3Dive