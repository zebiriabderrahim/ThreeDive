//
// Created by ABDERRAHIM ZEBIRI on 2024-06-24.
//

#ifndef THREEDIVE_GL_VERTEX_ARRAY_H
#define THREEDIVE_GL_VERTEX_ARRAY_H


namespace s3Dive {

    class GLVertexBuffer;
    class GLVertexBufferLayout;

    class GLVertexArray {
    public:
        GLVertexArray();
        ~GLVertexArray();

        void addBufferArray(const GLVertexBuffer &vbo, const GLVertexBufferLayout &layout) const;

        void bind() const;
        void unbind() const;

    private:
        GLuint rendererID_{};

    };

} // s3Dive

#endif //THREEDIVE_GL_VERTEX_ARRAY_H
