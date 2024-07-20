//
// Created by ABDERRAHIM ZEBIRI on 2024-06-26.
//

#ifndef THREEDIVE_GL_INDEX_BUFFER_H
#define THREEDIVE_GL_INDEX_BUFFER_H

#include <vector>

namespace s3Dive {

    class GLIndexBuffer {
    public:
        explicit GLIndexBuffer(const std::vector<unsigned int> &data);
        ~GLIndexBuffer();

        void bind() const;
        void unbind() const;

        [[nodiscard]] GLuint getCount() const;

    private:
        GLuint rendererID_{};
        GLuint count_;

    };

} // s3Dive

#endif //THREEDIVE_GL_INDEX_BUFFER_H
