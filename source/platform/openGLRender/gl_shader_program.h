//
// Created by ABDERRAHIM ZEBIRI on 2024-06-24.
//
#pragma once

#ifndef THREEDIVE_GL_SHADER_PROGRAM_H
#define THREEDIVE_GL_SHADER_PROGRAM_H


#include <string>
#include <string_view>
#include <unordered_map>
#include <glm/gtc/type_ptr.hpp>

#include "gl_shader.h"

namespace s3Dive {

    class GLShader;

    class GLShaderProgram {

    public:
        GLShaderProgram();
        ~GLShaderProgram();

        void initFromFiles(std::string_view vsPath, std::string_view fsPath) const;
        void initFromFiles(std::string_view vsPath, std::string_view gsPath, std::string_view fsPath) const;

        void use() const;
        void unuse() const;

        [[nodiscard]] GLint getAttribLocation(std::string_view name, bool verbose = true);
        [[nodiscard]] GLint getUniformLocation(std::string_view name);

        template<typename T> void updateShaderUniform(std::string_view name, T val);
        template<typename T> void updateShaderUniform(std::string_view name, T val1, T val2);
        template<typename T>void updateShaderUniform(std::string_view name, T val1, T val2, T val3);

    private:
        void checkLinkingErr() const;
        void link() const;
        void attachShader(const GLShader &shader) const;

        GLuint programId_;
        std::unordered_map<std::string, GLint> uniformLocationCache_;
        std::unordered_map<std::string, GLint> attribLocationCache_;
    };

} // s3Dive

#endif //THREEDIVE_GL_SHADER_PROGRAM_H
