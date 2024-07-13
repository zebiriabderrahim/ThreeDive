//
// Created by ABDERRAHIM ZEBIRI on 2024-06-24.
//
#pragma once

#ifndef THREEDIVE_GL_VERTEX_BUFFER_LAYOUT_H
#define THREEDIVE_GL_VERTEX_BUFFER_LAYOUT_H

#include <glad/glad.h>
#include <vector>
#include <stdexcept>

namespace s3Dive {

    struct VertexBufferElement {
        GLuint type;
        GLint count;
        GLboolean isNormalized;

        // Returns the size in bytes of a given OpenGL type
        static unsigned int getSizeOfType(GLuint type) {
            switch (type) {
                case GL_FLOAT:
                case GL_UNSIGNED_INT:
                    return 4;
                case GL_UNSIGNED_BYTE:
                    return 1;
                default:
                    throw std::invalid_argument("Unsupported type in VertexBufferElement");
            }
        }
    };

    class GLVertexBufferLayout {

    public:
        GLVertexBufferLayout() = default;

        template<typename T>
        void addVertexElement([[maybe_unused]] GLint count);

        [[nodiscard]] const std::vector<VertexBufferElement> &getElements() const { return elements_; }

        [[nodiscard]] GLsizei getStride() const;

    private:
        std::vector<VertexBufferElement> elements_;
        unsigned int stride_ = 0;
    };

} // s3Dive

#endif //THREEDIVE_GL_VERTEX_BUFFER_LAYOUT_H
