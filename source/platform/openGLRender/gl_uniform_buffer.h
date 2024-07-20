//
// Created by ABDERRAHIM ZEBIRI on 2024-06-28.
//

#ifndef THREEDIVE_GL_UNIFORM_BUFFER_H
#define THREEDIVE_GL_UNIFORM_BUFFER_H


namespace s3Dive {

    class GLUniFormBuffer {
    public:
        GLUniFormBuffer(uint32_t size, uint32_t binding);

        ~GLUniFormBuffer();

        template<typename T>
        void SetData(const std::vector<T> &data, uint32_t offset = 0);

    private:
        GLuint rendererID_{};
    };

} // s3Dive

#endif //THREEDIVE_GL_UNIFORM_BUFFER_H
