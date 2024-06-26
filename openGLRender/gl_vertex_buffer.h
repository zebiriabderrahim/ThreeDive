//
// Created by ABDERRAHIM ZEBIRI on 2024-06-25.
//

#ifndef THREEDIVE_GL_VERTEX_BUFFER_H
#define THREEDIVE_GL_VERTEX_BUFFER_H

#include <vector>

namespace s3Dive {

    class GLVertexBuffer {

    public:
        explicit GLVertexBuffer(const std::vector<float> &data);

        ~GLVertexBuffer();

        void bind() const;

        void unbind() const;

    private:
        unsigned int rendererID_{};

    };

} // s3Dive

#endif //THREEDIVE_GL_VERTEX_BUFFER_H
