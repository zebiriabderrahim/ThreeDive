//
// Created by ABDERRAHIM ZEBIRI on 2024-06-29.
//

#ifndef THREEDIVE_GL_RENDERER_H
#define THREEDIVE_GL_RENDERER_H

namespace s3Dive {

    class GLVertexArray;

    class GLRenderer {

    public:

        enum class BufferBit : GLbitfield {
            Color = GL_COLOR_BUFFER_BIT,
            Depth = GL_DEPTH_BUFFER_BIT,
            Stencil = GL_STENCIL_BUFFER_BIT
        };


        GLRenderer() = default;
        ~GLRenderer() = default;

        void init() const;

        void clear(BufferBit bit) const;
        void clear(BufferBit bit1, BufferBit bit2) const;
        void clear(BufferBit bit1, BufferBit bit2, BufferBit bit3) const;

        void setViewport(int x, int y, int width, int height) const;
        void setClearColor(const glm::vec4 &color) const;
        void setLineWidth(float width) const;

        void drawIndexed(const GLVertexArray &vao, GLuint indexCount = 0) const;
        void drawLines(GLint indexCount) const;

    };

} // s3Dive


#endif //THREEDIVE_GL_RENDERER_H
