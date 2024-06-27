//
// Created by ABDERRAHIM ZEBIRI on 2024-06-24.
//
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include "shader_program.h"

namespace s3Dive {

    ShaderProgram::ShaderProgram() {
        programId_ = glCreateProgram();
    }

    ShaderProgram::~ShaderProgram() {
        glDeleteProgram(programId_);
    }

    void ShaderProgram::initFromFiles(std::string_view vsPath, std::string_view fsPath) const {
       auto vertexShader = Shader(GL_VERTEX_SHADER, vsPath.data());
       auto fragmentShader = Shader(GL_FRAGMENT_SHADER, fsPath.data());

        attachShader(vertexShader);
        attachShader(fragmentShader);

        link();

    }

    void ShaderProgram::initFromFiles(std::string_view vsPath, std::string_view gsPath, std::string_view fsPath) const {

        auto vertexShader = Shader(GL_VERTEX_SHADER, Shader::loadShaderFile(vsPath.data()));
        auto geometryShader = Shader(GL_GEOMETRY_SHADER, Shader::loadShaderFile(gsPath.data()));
        auto fragmentShader = Shader(GL_FRAGMENT_SHADER, Shader::loadShaderFile(fsPath.data()));

        attachShader(vertexShader);
        attachShader(geometryShader);
        attachShader(fragmentShader);

        link();
    }

    void ShaderProgram::use() const {
        glUseProgram(programId_);
    }

    void ShaderProgram::unuse() {
        glUseProgram(0);
    }

    void ShaderProgram::link() const {
        glLinkProgram(programId_);
        checkLinkingErr();
    }

    void ShaderProgram::attachShader(const Shader& shader) const{
        glAttachShader(programId_, shader.getShaderId());
    }

    GLint ShaderProgram::getAttribLocation(std::string_view name, bool verbose)  {
        if (auto it = attribLocationCache_.find(name.data()); it != attribLocationCache_.end()) {
            return it->second;
        }
        GLint location = glGetAttribLocation(programId_, std::string(name).c_str());
        if (location == -1 && verbose) {
            spdlog::error("Attribute {} not found in shader program", name);
        }
        attribLocationCache_[std::string(name)] = location;
        return location;
    }

    GLint ShaderProgram::getUniformLocation(std::string_view name)  {
        if (auto it = uniformLocationCache_.find(name.data()); it != uniformLocationCache_.end()) {
            return it->second;
        }
        GLint location = glGetUniformLocation(programId_, std::string(name).c_str());
        if (location == -1) {
            spdlog::error("Uniform {} not found in shader program", name);
        }
        uniformLocationCache_[std::string(name)] = location;
        return location;
    }

    template<>
    void ShaderProgram::setUniform<int>(const std::string& name, int val) {
        glUniform1i(glGetUniformLocation(programId_, name.c_str()), val);
    }

    template<>
    void ShaderProgram::setUniform<bool>(const std::string& name, bool val) {
        glUniform1i(glGetUniformLocation(programId_, name.c_str()), val);
    }

    template<>
    void ShaderProgram::setUniform<float>(const std::string& name, float val) {
        glUniform1f(glGetUniformLocation(programId_, name.c_str()), val);
    }

    template<>
    void ShaderProgram::setUniform<float>(const std::string& name, float val1, float val2) {
        glUniform2f(glGetUniformLocation(programId_, name.c_str()), val1, val2);
    }

    template<>
    void ShaderProgram::setUniform<float>(const std::string& name, float val1, float val2, float val3) {
        glUniform3f(glGetUniformLocation(programId_, name.c_str()), val1, val2, val3);
    }

    template<>
    void ShaderProgram::setUniform<float*>(const std::string& name, float* val) {
        glUniformMatrix4fv(glGetUniformLocation(programId_, name.c_str()), 1, GL_FALSE, val);
    }

    void ShaderProgram::checkLinkingErr() const {
        GLint result;
        glGetProgramiv(programId_, GL_LINK_STATUS, &result);

        if (result == GL_FALSE) {
            GLint length;
            glGetProgramiv(programId_, GL_INFO_LOG_LENGTH, &length);

            std::vector<char> message(length);
            glGetProgramInfoLog(programId_, length, &length, message.data());
            spdlog::error("Failed to link shader program: {}", message.data());
            glDeleteProgram(programId_);
        }
    }

} // s3Dive