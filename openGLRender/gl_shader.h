//
// Created by ABDERRAHIM ZEBIRI on 2024-06-22.
//

#ifndef THREEDIVE_GL_SHADER_H
#define THREEDIVE_GL_SHADER_H

#include <string>

namespace s3Dive {

    class GLShader {
    public:
        GLShader(GLenum type, std::string_view filepath);
        ~GLShader();

        [[nodiscard]] GLuint getShaderId() const;

        [[nodiscard]] static std::string loadShaderFile(std::string_view filepath);

    private:
        void checkShaderError() const;

        GLuint shaderId_;
    };

} // s3Dive

#endif //THREEDIVE_GL_SHADER_H
