//
// Created by ABDERRAHIM ZEBIRI on 2024-06-24.
//
#include <vector>

#include <glad/glad.h>

#include "gl_vertex_buffer_layout.h"

namespace s3Dive {

    template<typename T>
    void GLVertexBufferLayout::addVertexElement([[maybe_unused]] GLint count) {
        static_assert(std::is_same<T, float>::value ||
                      std::is_same<T, unsigned int>::value ||
                      std::is_same<T, unsigned char>::value,
                      "Unsupported type for GLVertexBufferLayout::addVertexElement");
    }

    template<>
    void GLVertexBufferLayout::addVertexElement<float>(GLint count) {
        elements_.push_back({GL_FLOAT, count, GL_FALSE});
        stride_ += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
    }

    template<>
    void GLVertexBufferLayout::addVertexElement<unsigned int>(GLint count) {
        elements_.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
        stride_ += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
    }

    template<>
    void GLVertexBufferLayout::addVertexElement<unsigned char>(GLint count) {
        elements_.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
        stride_ += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
    }

    GLsizei GLVertexBufferLayout::getStride() const {
        return (GLsizei) stride_;
    }
} // s3Dive