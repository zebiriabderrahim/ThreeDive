//
// Created by ABDERRAHIM ZEBIRI on 2024-06-29.
//

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "gl_vertex_array.h"
#include "gl_renderer.h"


namespace s3Dive {
    void GLRenderer::init() const {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    }

    void GLRenderer::clear() const {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void GLRenderer::setViewport(int x, int y, int width, int height) const {
        glViewport(x, y, width, height);
    }

    void GLRenderer::setClearColor(const glm::vec4 &color) const {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void GLRenderer::setLineWidth(float width) const {
        glLineWidth(width);
    }

    void GLRenderer::drawIndexed(const GLVertexArray &vao, GLuint indexCount) const {
        vao.bind();
        auto count = indexCount ? indexCount : vao.getIndexBuffer()->getCount();
        glDrawElements(GL_TRIANGLES, static_cast<GLint>(count), GL_UNSIGNED_INT, nullptr);
    }

    void GLRenderer::drawLines(const GLVertexArray &vao, GLint indexCount) const {
        vao.bind();
        glDrawArrays(GL_LINES, 0, indexCount);
    }
} // s3Dive