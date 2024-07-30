//
// Created by ABDERRAHIM ZEBIRI on 2024-07-21.
//

#ifndef THREEDIVE_COMPONENTS_H
#define THREEDIVE_COMPONENTS_H

#include <string>
#include "../core/geo_generator.h"
#include "../core/uuid.h"
#include "../platform/openGLRender/gl_texture.h"
#include "../platform/openGLRender/gl_vertex_array.h"

namespace s3Dive {

    struct TransformComponent {
        glm::vec3 position{0.0f, 0.0f, 0.0f};
        glm::vec3 rotation{0.0f, 0.0f, 0.0f};
        glm::vec3 scale{1.0f, 1.0f, 1.0f};
    };
    struct MaterialComponent {
        glm::vec3 ambient{0.1f, 0.1f, 0.1f};
        glm::vec3 diffuse{0.8f, 0.8f, 0.8f};
        glm::vec3 specular{0.5f, 0.5f, 0.5f};
        float shininess{32.0f};
        std::shared_ptr<GLTexture> diffuseTexture;
        bool useTexture{false};

        MaterialComponent() = default;
        MaterialComponent(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shin)
                : ambient(amb), diffuse(diff), specular(spec), shininess(shin) {}
    };

    struct LightComponent {
        glm::vec3 position{0.0f, 5.0f, 5.0f};
        glm::vec3 color{1.0f, 1.0f, 1.0f};
        float intensity{1.0f};
        enum class LightType { Key, Fill, Point, Directional, Spot } type{LightType::Key};

        LightComponent() = default;
        LightComponent(glm::vec3 pos, glm::vec3 col, float intens, LightType t)
                : position(pos), color(col), intensity(intens), type(t) {}
    };

    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

    struct MeshComponent {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::shared_ptr<GLVertexBuffer> vertexBuffer;
        std::shared_ptr<GLVertexArray> vertexArray;
        std::shared_ptr<GLTexture> texture;

        glm::vec3 ambientColor = glm::vec3(0.1f, 0.1f, 0.1f);
        glm::vec3 diffuseColor = glm::vec3(0.8f, 0.8f, 0.8f);
        glm::vec3 specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
        float shininess = 32.0f;
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


} // s3Dive

#endif //THREEDIVE_COMPONENTS_H
