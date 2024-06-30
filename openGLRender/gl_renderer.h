//
// Created by ABDERRAHIM ZEBIRI on 2024-06-29.
//

#ifndef THREEDIVE_GL_RENDERER_H
#define THREEDIVE_GL_RENDERER_H

#include "gl_vertex_array.h"


namespace s3Dive {

    class GLRenderer {
    public:
        GLRenderer() = default;

        ~GLRenderer() = default;

        void init() const;

        void clear() const;

        void setviewport(int x, int y, int width, int height) const;

        void setClearColor(const glm::vec4 &color) const;

        void setlineWidth(float width) const;

        void drawIndexed(const GLVertexArray &vao, GLuint indexCount) const;

        void drawlines(const GLVertexArray &vao, GLuint indexCount) const;

    };

} // s3Dive

#endif //THREEDIVE_GL_RENDERER_H
