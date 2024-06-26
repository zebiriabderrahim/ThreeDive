//
// Created by ABDERRAHIM ZEBIRI on 2024-06-24.
//
#include <glad/glad.h>

#include "gl_vertex_array.h"
#include "gl_vertex_buffer.h"
#include "gl_vertex_buffer_layout.h"



namespace s3Dive {
    GLVertexArray::GLVertexArray() {
        glGenVertexArrays(1, &rendererID_);
    }

    GLVertexArray::~GLVertexArray() {
        glDeleteVertexArrays(1, &rendererID_);
    }

    void GLVertexArray::addBufferArray(const GLVertexBuffer &vbo, const GLVertexBufferLayout &layout) const {
        bind();
        vbo.bind();

        const auto& elements = layout.getElements();
        unsigned int offset = 0;
        const auto stride = layout.getStride();

        for (unsigned int i = 0; i < elements.size(); ++i) {
            const auto& element = elements[i];

            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, element.count, element.type, element.isNormalized, stride,
                                  (const void *)(uintptr_t)(offset));
            offset += element.count * VertexBufferElement::getSizeOfType(element.type);
        }
    }

    void GLVertexArray::bind() const {
        glBindVertexArray(rendererID_);
    }

    void GLVertexArray::unbind() const {
        glBindVertexArray(0);
    }
} // s3Dive