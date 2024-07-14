//
// Created by ABDERRAHIM ZEBIRI on 2024-06-25.
//

#include "gl_vertex_buffer.h"

namespace s3Dive {
    GLVertexBuffer::GLVertexBuffer(const std::vector<float> &data) {
        glGenBuffers(1, &rendererID_);
        glBindBuffer(GL_ARRAY_BUFFER, rendererID_);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.size() * sizeof(float)), data.data(),
                     GL_STATIC_DRAW);
    }

    GLVertexBuffer::GLVertexBuffer(const std::vector<glm::vec3> &data) {
        glGenBuffers(1, &rendererID_);
        glBindBuffer(GL_ARRAY_BUFFER, rendererID_);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.size() * sizeof(glm::vec3)), data.data(),
                     GL_STATIC_DRAW);
    }

    GLVertexBuffer::~GLVertexBuffer() {
        glDeleteBuffers(1, &rendererID_);
    }

    void GLVertexBuffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, rendererID_);
    }

    void GLVertexBuffer::unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void GLVertexBuffer::setLayout(const GLVertexBufferLayout &layout) {
        layout_ = layout;
    }

    GLVertexBufferLayout &GLVertexBuffer::getLayout() {
        return layout_;
    }
} // s3Dive