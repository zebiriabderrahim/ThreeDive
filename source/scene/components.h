//
// Created by ABDERRAHIM ZEBIRI on 2024-07-21.
//

#ifndef THREEDIVE_COMPONENTS_H
#define THREEDIVE_COMPONENTS_H

#include <string>
#include "../core/geo_generator.h"

namespace s3Dive {

    struct TransformComponent {
        glm::vec3 position{0.0f, 0.0f, 0.0f};
        glm::vec3 rotation{0.0f, 0.0f, 0.0f};
        glm::vec3 scale{1.0f, 1.0f, 1.0f};
    };

    struct MeshComponent {
        std::string meshFilePath;
        // Add other mesh-related properties here, like material, textures, etc.
    };

    struct SceneGridComponent {
        float size_ = 10.0f;
        float step_ = 1.0f;
        float extensionSize_ = 0.1f;
        int fadeSteps_ = 3;
        bool isInitialized{false};
        std::vector<float> vertices;

        explicit SceneGridComponent(float size, float step, float extensionSize, int fadeSteps)
                : size_(size), step_(step), extensionSize_(extensionSize), fadeSteps_(fadeSteps),
                vertices(geo::generateDetailedGridVertices(size_, step_, extensionSize_, fadeSteps_)) {}
    };


} // s3Dive

#endif //THREEDIVE_COMPONENTS_H
