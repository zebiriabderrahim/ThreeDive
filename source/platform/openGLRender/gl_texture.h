//
// Created by ABDERRAHIM ZEBIRI on 2024-06-26.
//
#pragma once

#ifndef THREEDIVE_GL_TEXTURE_H
#define THREEDIVE_GL_TEXTURE_H

namespace s3Dive {

    class GLTexture {
    public:
        explicit GLTexture(std::string_view path);
        ~GLTexture();

        void bind(unsigned int slot = 0) const;
        void unbind() const;

        [[nodiscard]] inline int getWidth() const { return width_; }
        [[nodiscard]] inline int getHeight() const { return height_; }

    private:
        GLuint rendererID_{};
        unsigned char *localBuffer_ = nullptr;
        int width_{};
        int height_{};
        int BPP_{};

    };

} // s3Dive

#endif //THREEDIVE_GL_TEXTURE_H
