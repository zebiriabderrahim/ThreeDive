//
// Created by ABDERRAHIM ZEBIRI on 2024-07-21.
//

#ifndef THREEDIVE_COMPONENTS_H
#define THREEDIVE_COMPONENTS_H

#include <string>
#include <vector>
#include <memory>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "../core/geo_generator.h"
#include "../core/uuid.h"
#include "../platform/openGLRender/gl_texture.h"
#include "../platform/openGLRender/gl_vertex_array.h"

namespace s3Dive {

    struct TransformComponent {
        glm::vec3 Translation{0.0f, 0.0f, 0.0f};
        glm::vec3 Rotation{0.0f, 0.0f, 0.0f};
        glm::vec3 Scale{1.0f, 1.0f, 1.0f};

        TransformComponent() = default;
        TransformComponent(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale)
                : Translation(translation), Rotation(rotation), Scale(scale) {}

        glm::mat4 GetTransform() const {
            glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
            return glm::translate(glm::mat4(1.0f), Translation)
                   * rotation
                   * glm::scale(glm::mat4(1.0f), Scale);
        }
    };


    struct MaterialComponent {
        glm::vec3 albedo = glm::vec3(0.435f, 0.435f, 0.435f);
        float metallic = 0.1f;
        float roughness = 0.5f;
        float ao = 1.0f;
        std::shared_ptr<GLTexture> diffuseTexture;
    };

    enum class LightType {
        Directional,
        Point,
        Spot
    };

    struct LightComponent {
        LightType type = LightType::Point;
        glm::vec3 position = glm::vec3(0.0f);
        glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);
        glm::vec3 color = glm::vec3(1.0f);
        float intensity = 1.0f;
        float range = 10.0f;  // For point and spot lights
        float spotAngle = 45.0f;  // For spot lights
    };

    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

    struct MeshComponent {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::shared_ptr<GLVertexArray> vertexArray;
        bool isInitialized = false;
    };

    struct ModelComponent {
        std::string filepath;
        std::vector<s3Dive::UUID> meshEntities;  // UUIDs of associated mesh entities
    };

    struct SceneGridComponent {
        float size_ = 10.0f;
        float step_ = 1.0f;
        float extensionSize_ = 0.1f;
        int fadeSteps_ = 3;
        bool isInitialized{false};
        std::vector<float> vertices;

        explicit SceneGridComponent(float size= 10.0f, float step = 2.0f, float extensionSize = 4.0f, int fadeSteps = 5)
                : size_(size), step_(step), extensionSize_(extensionSize), fadeSteps_(fadeSteps),
                vertices(geo::generateDetailedGridVertices(size_, step_, extensionSize_, fadeSteps_)) {}
    };

} // namespace s3Dive

#endif //THREEDIVE_COMPONENTS_H