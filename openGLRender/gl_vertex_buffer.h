//
// Created by ABDERRAHIM ZEBIRI on 2024-06-25.
//

#ifndef THREEDIVE_GL_VERTEX_BUFFER_H
#define THREEDIVE_GL_VERTEX_BUFFER_H

#include <vector>
#include "gl_vertex_buffer_layout.h"


namespace s3Dive {

    class GLVertexBuffer {

    public:
        explicit GLVertexBuffer(const std::vector<float> &data);

        ~GLVertexBuffer();

        void bind() const;

        void unbind() const;

        void setLayout(const GLVertexBufferLayout &layout);

        GLVertexBufferLayout &getLayout();

    private:
        unsigned int rendererID_{};
        GLVertexBufferLayout layout_;
    };

} // s3Dive

#endif //THREEDIVE_GL_VERTEX_BUFFER_H
