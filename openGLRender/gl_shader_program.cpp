//
// Created by ABDERRAHIM ZEBIRI on 2024-06-24.
//
#include <spdlog/spdlog.h>

#include "gl_shader.h"
#include "gl_shader_program.h"

namespace s3Dive {

    GLShaderProgram::GLShaderProgram() {
        programId_ = glCreateProgram();
    }

    GLShaderProgram::~GLShaderProgram() {
        glDeleteProgram(programId_);
    }

    void GLShaderProgram::initFromFiles(std::string_view vsPath, std::string_view fsPath) const {
        auto vertexShader = GLShader(GL_VERTEX_SHADER, vsPath.data());
        auto fragmentShader = GLShader(GL_FRAGMENT_SHADER, fsPath.data());

        attachShader(vertexShader);
        attachShader(fragmentShader);

        link();

    }

    void
    GLShaderProgram::initFromFiles(std::string_view vsPath, std::string_view gsPath, std::string_view fsPath) const {

        auto vertexShader = GLShader(GL_VERTEX_SHADER, GLShader::loadShaderFile(vsPath.data()));
        auto geometryShader = GLShader(GL_GEOMETRY_SHADER, GLShader::loadShaderFile(gsPath.data()));
        auto fragmentShader = GLShader(GL_FRAGMENT_SHADER, GLShader::loadShaderFile(fsPath.data()));

        attachShader(vertexShader);
        attachShader(geometryShader);
        attachShader(fragmentShader);

        link();
    }

    void GLShaderProgram::use() const {
        glUseProgram(programId_);
    }

    void GLShaderProgram::unuse() const {
        glUseProgram(0);
    }

    void GLShaderProgram::link() const {
        glLinkProgram(programId_);
        checkLinkingErr();
    }

    void GLShaderProgram::attachShader(const GLShader &shader) const {
        glAttachShader(programId_, shader.getShaderId());
    }

    GLint GLShaderProgram::getAttribLocation(std::string_view name, bool verbose) {
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

    GLint GLShaderProgram::getUniformLocation(std::string_view name) {
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
    void GLShaderProgram::updateShaderUniform<int>(std::string_view name, int val) {
        GLint location = getUniformLocation(name);
        glUniform1i(location, val);
    }

    template<>
    void GLShaderProgram::updateShaderUniform<bool>(std::string_view name, bool val) {
        GLint location = getUniformLocation(name);
        glUniform1i(location, val);
    }

    template<>
    void GLShaderProgram::updateShaderUniform<float>(std::string_view name, float val) {
        GLint location = getUniformLocation(name);
        glUniform1f(location, val);
    }

    template<>
    void GLShaderProgram::updateShaderUniform<float>(std::string_view name, float val1, float val2) {
        GLint location = getUniformLocation(name);
        glUniform2f(location, val1, val2);
    }

    template<>
    void GLShaderProgram::updateShaderUniform<float>(std::string_view name, float val1, float val2, float val3) {
        GLint location = getUniformLocation(name);
        glUniform3f(location, val1, val2, val3);
    }

    template<>
    void GLShaderProgram::updateShaderUniform<glm::vec3>(std::string_view name, glm::vec3 val) {
        GLint location = getUniformLocation(name);
        glUniform3f(location, val.x, val.y, val.z);
    }

    template<>
    void GLShaderProgram::updateShaderUniform<float *>(std::string_view name, float *val) {
        GLint location = getUniformLocation(name);
        glUniformMatrix4fv(location, 1, GL_FALSE, val);
    }

    void GLShaderProgram::checkLinkingErr() const {
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