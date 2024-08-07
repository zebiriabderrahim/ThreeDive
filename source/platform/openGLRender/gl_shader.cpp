//
// Created by ABDERRAHIM ZEBIRI on 2024-06-22.
//
#include <fstream>
#include <sstream>
#include <spdlog/spdlog.h>

#include "gl_shader.h"


namespace s3Dive {

    GLShader::GLShader(GLenum type, std::string_view filepath) {
        shaderId_ = glCreateShader(type);
        const std::string& source = loadShaderFile(filepath);
        const char *src = source.c_str();
        glShaderSource(shaderId_, 1, &src, nullptr);
        glCompileShader(shaderId_);
        checkShaderError();
    }

    GLShader::~GLShader() {
        glDeleteShader(shaderId_);
    }

    GLuint GLShader::getShaderId() const {
        return shaderId_;
    }

    void GLShader::checkShaderError() const {
        int result;
        glGetShaderiv(shaderId_, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) {
            int length;
            glGetShaderiv(shaderId_, GL_INFO_LOG_LENGTH, &length);

            std::vector<char> message(length);

            glGetShaderInfoLog(shaderId_, length, &length, message.data());
            spdlog::error("Failed to compile shader: {}", message.data());
            glDeleteShader(shaderId_);
        }
    }


    std::string GLShader::loadShaderFile(std::string_view filepath) {
        try {
            std::ifstream stream(filepath.data());
            if (!stream.is_open()) {
                throw std::ifstream::failure("Failed to open file: " + std::string(filepath));
            }
            std::stringstream ss;
            ss << stream.rdbuf();
            return ss.str();
        } catch (const std::ifstream::failure &e) {
            spdlog::error("Failed to parse shader: {}", e.what());
            return "";
        }
    }


} // s3Dive