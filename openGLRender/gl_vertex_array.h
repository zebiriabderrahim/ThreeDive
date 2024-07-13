//
// Created by ABDERRAHIM ZEBIRI on 2024-06-24.
//
#pragma once

#ifndef THREEDIVE_GL_VERTEX_ARRAY_H
#define THREEDIVE_GL_VERTEX_ARRAY_H

#include <vector>
#include <memory>

#include "gl_vertex_buffer.h"
#include "gl_index_buffer.h"

namespace s3Dive {

    class GLVertexArray {

    public:
        GLVertexArray();
        ~GLVertexArray();

        void bind() const;
        void unbind() const;

        void addVertexBuffer(const std::shared_ptr<GLVertexBuffer> &vbo);
        void setIndexBuffer(const std::shared_ptr<GLIndexBuffer> &ibo);

        [[nodiscard]] const std::vector<std::shared_ptr<GLVertexBuffer>> &getVertexBuffers() const { return vertexBuffers_; }
        [[nodiscard]] const std::shared_ptr<GLIndexBuffer> &getIndexBuffer() const { return indexBuffer_; }

    private:
        GLuint rendererID_{};
        std::vector<std::shared_ptr<GLVertexBuffer>> vertexBuffers_;
        std::shared_ptr<GLIndexBuffer> indexBuffer_;

    };

} // s3Dive

#endif //THREEDIVE_GL_VERTEX_ARRAY_H
