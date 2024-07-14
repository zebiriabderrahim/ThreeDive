//
// Created by ABDERRAHIM ZEBIRI on 2024-06-29.
//
#pragma once

#ifndef THREEDIVE_GL_RENDERER_H
#define THREEDIVE_GL_RENDERER_H

namespace s3Dive {

    class GLVertexArray;

    class GLRenderer {
    public:
        GLRenderer() = default;
        ~GLRenderer() = default;

        void init() const;
        void clear() const;

        void setViewport(int x, int y, int width, int height) const;
        void setClearColor(const glm::vec4 &color) const;
        void setLineWidth(float width) const;

        void drawIndexed(const GLVertexArray &vao, GLuint indexCount) const;
        void drawLines(const GLVertexArray &vao, GLint indexCount) const;
    };

} // s3Dive


#endif //THREEDIVE_GL_RENDERER_H
