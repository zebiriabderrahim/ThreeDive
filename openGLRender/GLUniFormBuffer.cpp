//
// Created by ABDERRAHIM ZEBIRI on 2024-06-28.
//
#include <vector>
#include "glad/glad.h"

#include "GLUniFormBuffer.h"

namespace s3Dive {

    GLUniFormBuffer::GLUniFormBuffer(uint32_t size, uint32_t binding) {
        glGenBuffers(1, &rendererID_);
        glBindBuffer(GL_UNIFORM_BUFFER, rendererID_);
        glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, binding, rendererID_);
    }

    GLUniFormBuffer::~GLUniFormBuffer() {
        glDeleteBuffers(1, &rendererID_);
    }

    template<typename T>
    void GLUniFormBuffer::SetData(const std::vector<T> &data, uint32_t offset) {
        glBindBuffer(GL_UNIFORM_BUFFER, rendererID_);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, data.size() * sizeof(T), data.data());
    }

} // s3Dive