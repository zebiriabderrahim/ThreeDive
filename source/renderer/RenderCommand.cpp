//
// Created by ABDERRAHIM ZEBIRI on 2024-07-12.
//

#include "RenderCommand.h"
#include "../core/memory_and_binding.h"


namespace s3Dive {
    void RenderCommand::init() noexcept {
        getRenderer().init();
    }

    void RenderCommand::clear(GLRenderer::BufferBit bit) noexcept {
        getRenderer().clear(bit);
    }

    void RenderCommand::clear(GLRenderer::BufferBit bit1, GLRenderer::BufferBit bit2) noexcept {
        getRenderer().clear(bit1, bit2);
    }

    void RenderCommand::clear(GLRenderer::BufferBit bit1, GLRenderer::BufferBit bit2, GLRenderer::BufferBit bit3) noexcept {
        getRenderer().clear(bit1, bit2, bit3);
    }

    void RenderCommand::setViewport(int x, int y, int width, int height) noexcept {
        getRenderer().setViewport(x, y, width, height);
    }

    void RenderCommand::setClearColor(const glm::vec4 &color) noexcept {
        getRenderer().setClearColor(color);
    }

    void RenderCommand::setLineWidth(float width) noexcept {
        getRenderer().setLineWidth(width);
    }

    void RenderCommand::drawIndexed(const GLVertexArray& vao,GLuint indexCount) noexcept {
        vao.bind();
        getRenderer().drawIndexed(vao, indexCount);
    }

     void RenderCommand::drawLines(const GLVertexArray& vao, GLsizei indexCount) noexcept {
        vao.bind();
        getRenderer().drawLines(indexCount);
    }

    GLRenderer &RenderCommand::getRenderer() noexcept {
        static const auto rendererApi = MakeUnique<GLRenderer>();
        return *rendererApi;
    }
} // s3Dive