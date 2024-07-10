#include <glad/glad.h>
#include <glm/glm.hpp>
#include "SceneGrid.h"
#include "../openGLRender/gl_renderer.h"


namespace s3Dive {

    SceneGrid::SceneGrid(int size, float step, float extensionSize, int fadeSteps)
            : size_(size), step_(step), extensionSize_(extensionSize), fadeSteps_(fadeSteps) {
        initializeOpenGLObjects();
    }

    void SceneGrid::render(GLShaderProgram& shaderProgram, UniquePtr<CameraController> &cameraController) const {
        if (!isInitialized_) {
            return;
        }

        shaderProgram.use();
        VAO_.bind();
        GLRenderer renderer;
        renderer.drawLines(VAO_, vertexCount_);
        shaderProgram.updateShaderUniform("detailVisibility1", cameraController->getDetailVisibility1());
        shaderProgram.updateShaderUniform("detailVisibility2", cameraController->getDetailVisibility2());
        VAO_.unbind();
        shaderProgram.unuse();
    }

    void SceneGrid::initializeOpenGLObjects() {
        std::vector<float> vertices = generateGridVertices();
        VBO_ = std::make_shared<GLVertexBuffer>(vertices);
        vertexCount_ = vertices.size() / 8; // 8 floats per vertex
        GLVertexBufferLayout layout;
        layout.addVertexElement<float>(3);
        layout.addVertexElement<float>(4);
        layout.addVertexElement<float>(1);
        VBO_->setLayout(layout);
        VAO_.addVertexBuffer(VBO_);

        isInitialized_ = true;
    }

    std::vector<float> SceneGrid::generateGridVertices() const {
        std::vector<float> vertices;
        int lineCount = static_cast<int>((2 * size_) / step_) + 1;
        vertices.reserve(lineCount * 2 * (2 + 4 * fadeSteps_) * 8); // Pre-allocate memory

        auto addLine = [&](const glm::vec3& start, const glm::vec3& end, float visibility) {
            float alpha = visibility >= 0.9f ? 1.0f : (visibility >= 0.4f ? 0.5f : 0.25f);
            glm::vec3 direction = glm::normalize(end - start);
            glm::vec3 extension = direction * (visibility >= 0.9f ? extensionSize_ : extensionSize_ * 0.5f);

            auto addVertex = [&](const glm::vec3& pos, float a) {
                vertices.insert(vertices.end(), {pos.x, pos.y, pos.z, 1.0f, 1.0f, 1.0f, a, visibility});
            };

            // Main line
            addVertex(start, alpha);
            addVertex(end, alpha);

            // Extensions
            for (int i = 0; i < fadeSteps_; ++i) {
                float t1 = static_cast<float>(i) / fadeSteps_;
                float t2 = static_cast<float>(i + 1) / fadeSteps_;
                float alpha1 = alpha * (1.0f - t1);
                float alpha2 = alpha * (1.0f - t2);

                // Start extension
                addVertex(start - extension * t1, alpha1);
                addVertex(start - extension * t2, alpha2);

                // End extension
                addVertex(end + extension * t1, alpha1);
                addVertex(end + extension * t2, alpha2);
            }
        };

        // Generate lines
        for (int i = 0; i < lineCount; ++i) {
            float pos = -size_ + i * step_;
            // Main grid lines
            addLine({pos, -size_, 0}, {pos, size_, 0}, 1.0f);
            addLine({-size_, pos, 0}, {size_, pos, 0}, 1.0f);

            if (i < lineCount - 1) {
                // First level of detail
                float detailPos1 = pos + step_ * 0.5f;
                addLine({detailPos1, -size_, 0}, {detailPos1, size_, 0}, 0.5f);
                addLine({-size_, detailPos1, 0}, {size_, detailPos1, 0}, 0.5f);

                // Second level of detail
                float detailPos2 = pos + step_ * 0.25f;
                float detailPos3 = pos + step_ * 0.75f;
                addLine({detailPos2, -size_, 0}, {detailPos2, size_, 0}, 0.0f);
                addLine({-size_, detailPos2, 0}, {size_, detailPos2, 0}, 0.0f);
                addLine({detailPos3, -size_, 0}, {detailPos3, size_, 0}, 0.0f);
                addLine({-size_, detailPos3, 0}, {size_, detailPos3, 0}, 0.0f);
            }
        }

        return vertices;
    }

} // s3dive
