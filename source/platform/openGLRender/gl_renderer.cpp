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

    void GLRenderer::clear(GLRenderer::BufferBit bit) const {
        glClear(static_cast<GLbitfield>(bit));
    }

    void GLRenderer::clear(GLRenderer::BufferBit bit1, GLRenderer::BufferBit bit2) const {
        glClear(static_cast<GLbitfield>(bit1) | static_cast<GLbitfield>(bit2));
    }

    void GLRenderer::clear(GLRenderer::BufferBit bit1, GLRenderer::BufferBit bit2, GLRenderer::BufferBit bit3) const {
        glClear(static_cast<GLbitfield>(bit1) | static_cast<GLbitfield>(bit2) | static_cast<GLbitfield>(bit3));
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
        auto count = indexCount ? indexCount : vao.getIndexBuffer()->getCount();
        glDrawElements(GL_TRIANGLES, static_cast<GLint>(count), GL_UNSIGNED_INT, nullptr);
    }

    void GLRenderer::drawLines(GLint indexCount) const {
        glDrawArrays(GL_LINES, 0, indexCount);
    }


} // s3Dive