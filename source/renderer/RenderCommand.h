//
// Created by ABDERRAHIM ZEBIRI on 2024-07-12.
//

#ifndef THREEDIVE_RENDERCOMMAND_H
#define THREEDIVE_RENDERCOMMAND_H

#include <glm/glm.hpp>
#include "../platform/openGLRender/gl_vertex_array.h"
#include "../platform/openGLRender/gl_renderer.h"


namespace s3Dive {

    class RenderCommand {
    public:
        RenderCommand() = delete;
        ~RenderCommand() = default;

        static void init() noexcept;
        static void clear() noexcept;

        static void setViewport(int x, int y, int width, int height) noexcept;
        static void setClearColor(const glm::vec4& color) noexcept;
        static void setLineWidth(float width) noexcept;

        static void drawIndexed(const GLVertexArray& vao, GLsizei indexCount) noexcept;
        static void drawLines(const GLVertexArray& vao, GLsizei indexCount) noexcept;

    private:
        [[nodiscard]] static GLRenderer& getRenderer() noexcept;
    };

} // s3Dive

#endif //THREEDIVE_RENDERCOMMAND_H
