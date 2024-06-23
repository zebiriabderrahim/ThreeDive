//
// Created by ABDERRAHIM ZEBIRI on 2024-06-22.
//

#ifndef THREEDIVE_SHADER_H
#define THREEDIVE_SHADER_H
#include <glad/glad.h>
#include <string>

namespace s3Dive {

    class Shader {
    public:
        Shader(GLenum type, const std::string& code);

        [[nodiscard]] GLuint getShaderId() const;

    private:
        void checkShaderError() const;
        [[nodiscard]] static std::string loadShaderFile(const std::string& filepath) ;
        GLuint shaderId_;
    };

} // s3Dive

#endif //THREEDIVE_SHADER_H
