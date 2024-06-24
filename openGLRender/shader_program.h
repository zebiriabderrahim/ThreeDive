//
// Created by ABDERRAHIM ZEBIRI on 2024-06-24.
//

#ifndef THREEDIVE_SHADER_PROGRAM_H
#define THREEDIVE_SHADER_PROGRAM_H

#include <glad/glad.h>

#include "shader.h"


namespace s3Dive {

    class ShaderProgram {

    public:
        ShaderProgram();
        ~ShaderProgram();

        void initFromFiles(std::string_view vsPath, std::string_view fsPath) const;
        void initFromFiles(std::string_view vsPath, std::string_view gsPath, std::string_view fsPath) const;

        void use() const;
        static void unuse() ;

        [[nodiscard]] GLint getAttribLocation(std::string_view name, bool verbose = true);
        [[nodiscard]] GLint getUniformLocation(std::string_view name);

        template<typename T> void setUniform(const std::string& name, T val);
        template<typename T> void setUniform(const std::string& name, T val1, T val2);
        template<typename T> void setUniform(const std::string& name, T val1, T val2, T val3);



    private:
        void checkLinkingErr() const;
        void link() const;
        void attachShader(const Shader& shader) const;

        GLuint programId_;
        std::unordered_map<std::string, GLint> uniformLocationCache_;
        std::unordered_map<std::string, GLint> attribLocationCache_;

    };

} // s3Dive

#endif //THREEDIVE_SHADER_PROGRAM_H
