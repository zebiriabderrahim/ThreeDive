//
// Created by ABDERRAHIM ZEBIRI on 2024-07-07.
//

#ifndef THREEDIVE_GEO_UTIL_H
#define THREEDIVE_GEO_UTIL_H
#pragma once

#include <vector>
#include <glm/glm.hpp>
namespace s3Dive {
    std::vector<float> generateDetailedGridVertices(float size, float step, float extensionSize, int fadeSteps) {
        std::vector<float> vertices;
        int lineCount = static_cast<int>((2 * size) / step) + 1;

        auto addLine = [&](const glm::vec3 &start, const glm::vec3 &end, float visibility, const glm::vec3 &color) {
            float alpha = visibility >= 0.9f ? 1.0f : (visibility >= 0.4f ? 0.5f : 0.25f);
            glm::vec3 direction = glm::normalize(end - start);
            glm::vec3 extension = direction * (visibility >= 0.9f ? extensionSize : extensionSize * 0.5f);

            auto addVertex = [&](const glm::vec3 &pos, float a) {
                vertices.insert(vertices.end(), {pos.x, pos.y, pos.z, color.r, color.g, color.b, a, visibility});
            };

            // Main line
            addVertex(start, alpha);
            addVertex(end, alpha);

            // Extensions
            for (int i = 0; i < fadeSteps; ++i) {
                float t1 = static_cast<float>(i) / fadeSteps;
                float t2 = static_cast<float>(i + 1) / fadeSteps;
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
            float pos = -size + i * step;
            glm::vec3 color(1.0f, 1.0f, 1.0f); // Default white color

            // Main grid lines
//            if (pos == 0) {
//                // X-axis (red)
//                addLine({-size, 0, 0}, {size, 0, 0}, 1.0f, glm::vec3(0.63f,0.125f,0.08f));
//                // Y-axis (green)
//                addLine({0, -size, 0}, {0, size, 0}, 1.0f, glm::vec3(0.3f,0.65f,0.2f));
//            } else {
                addLine({pos, -size, 0}, {pos, size, 0}, 1.0f, color);
                addLine({-size, pos, 0}, {size, pos, 0}, 1.0f, color);
//            }

            if (i < lineCount - 1) {
                // First level of detail
                float detailPos1 = pos + step * 0.5f;
                addLine({detailPos1, -size, 0}, {detailPos1, size, 0}, 0.5f, color);
                addLine({-size, detailPos1, 0}, {size, detailPos1, 0}, 0.5f, color);

                // Second level of detail
                float detailPos2 = pos + step * 0.25f;
                float detailPos3 = pos + step * 0.75f;
                addLine({detailPos2, -size, 0}, {detailPos2, size, 0}, 0.0f, color);
                addLine({-size, detailPos2, 0}, {size, detailPos2, 0}, 0.0f, color);
                addLine({detailPos3, -size, 0}, {detailPos3, size, 0}, 0.0f, color);
                addLine({-size, detailPos3, 0}, {size, detailPos3, 0}, 0.0f, color);
            }
        }

        return vertices;
    }

} // s3Dive
#endif //THREEDIVE_GEO_UTIL_H
