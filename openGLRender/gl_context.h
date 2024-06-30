//
// Created by ABDERRAHIM ZEBIRI on 2024-06-29.
//

#ifndef THREEDIVE_GL_CONTEXT_H
#define THREEDIVE_GL_CONTEXT_H

namespace s3Dive {

    class GLContext {

    public:
        explicit GLContext(GLFWwindow *window);

        void init() const;

        void swapBuffers() const;

    private:
        GLFWwindow *window_{};
    };

} // s3Dive

#endif //THREEDIVE_GL_CONTEXT_H
