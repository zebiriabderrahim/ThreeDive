//
// Created by ABDERRAHIM ZEBIRI on 2024-06-22.
//
#include <spdlog/spdlog.h>
#include <fstream>
#include <sstream>

#include "Shader.h"


namespace s3Dive {

    Shader::Shader(GLenum type, const std::string &filepath) {
        shaderId_ = glCreateShader(type);
        const char *src = filepath.c_str();
        glShaderSource(shaderId_, 1, &src, nullptr);
        glCompileShader(shaderId_);
        checkShaderError();
    }

    GLuint Shader::getShaderId() const {
        return shaderId_;
    }

    void Shader::checkShaderError() const {
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


    std::string Shader::loadShaderFile(const std::string &filepath) {
        try {
            std::ifstream stream(filepath);
            if (!stream.is_open()) {
                throw std::ifstream::failure("Failed to open file: " + filepath);
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