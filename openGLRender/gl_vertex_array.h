//
// Created by ABDERRAHIM ZEBIRI on 2024-06-24.
//

#ifndef THREEDIVE_GL_VERTEX_ARRAY_H
#define THREEDIVE_GL_VERTEX_ARRAY_H

#include <vector>
#include <memory>

#include "gl_index_buffer.h"
#include "gl_vertex_buffer.h"


namespace s3Dive {

    using VertexBufferObjectRef = std::shared_ptr<GLVertexBuffer>;
    using IndexBufferObjectRef = std::shared_ptr<GLIndexBuffer>;

    class GLVertexArray {

    public:
        GLVertexArray();
        ~GLVertexArray();

        void bind() const;
        void unbind() const;

        void addVertexBuffer(const VertexBufferObjectRef &vbo);
        void setIndexBuffer(const IndexBufferObjectRef &ibo);

        [[nodiscard]] const std::vector<VertexBufferObjectRef> &getVertexBuffers() const { return vertexBuffers_; }
        [[nodiscard]] const IndexBufferObjectRef &getIndexBuffer() const { return indexBuffer_; }

    private:
        GLuint rendererID_{};
        std::vector<VertexBufferObjectRef> vertexBuffers_;
        IndexBufferObjectRef indexBuffer_;

    };

} // s3Dive

#endif //THREEDIVE_GL_VERTEX_ARRAY_H
