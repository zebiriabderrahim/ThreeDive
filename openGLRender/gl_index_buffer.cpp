//
// Created by ABDERRAHIM ZEBIRI on 2024-06-26.
//
#include <vector>
#include <glad/glad.h>

#include "gl_index_buffer.h"

namespace s3Dive {
    GLIndexBuffer::GLIndexBuffer(const std::vector<unsigned int> &data) : count_(static_cast<GLuint>(data.size())) {
        glGenBuffers(1, &rendererID_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     static_cast<GLuint>(data.size() * sizeof(unsigned int)),
                     data.data(),
                     GL_STATIC_DRAW);
    }

    GLIndexBuffer::~GLIndexBuffer() {
        glDeleteBuffers(1, &rendererID_);
    }

    void GLIndexBuffer::bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID_);
    }

    void GLIndexBuffer::unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    GLuint GLIndexBuffer::getCount() const {
        return count_;
    }
} // s3Dive