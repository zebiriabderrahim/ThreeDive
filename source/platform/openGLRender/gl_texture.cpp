//
// Created by ABDERRAHIM ZEBIRI on 2024-06-26.
//
#include <spdlog/spdlog.h>
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "gl_texture.h"

namespace s3Dive {

    GLTexture::GLTexture(std::string_view path) {

        stbi_set_flip_vertically_on_load(1);

        localBuffer_ = stbi_load(path.data(), &width_, &height_, &BPP_, 4);
        auto isBufferLoaded = static_cast<bool>(localBuffer_);
        if (!isBufferLoaded) {
            spdlog::error("Failed to load texture: {}", path);
        }

        glGenTextures(1, &rendererID_);
        glBindTexture(GL_TEXTURE_2D, rendererID_);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGBA8,
                     width_,
                     height_,
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     localBuffer_);

        unbind();

        if (isBufferLoaded) {
            stbi_image_free(localBuffer_);
        }
    }

    GLTexture::~GLTexture() {
        glDeleteTextures(1, &rendererID_);
    }

    void GLTexture::bind(unsigned int slot) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, rendererID_);
    }

    void GLTexture::unbind() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
} // s3Dive