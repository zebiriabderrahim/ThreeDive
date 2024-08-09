//
// Created by ABDERRAHIM ZEBIRI on 2024-07-27.
//
#ifndef THREEDIVE_MESHLOADINGSYSTEM_H
#define THREEDIVE_MESHLOADINGSYSTEM_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../platform/openGLRender/gl_texture.h"
#include "../platform/openGLRender/gl_vertex_array.h"
#include "../platform/openGLRender/gl_vertex_buffer.h"
#include "system.h"
#include "components.h"

namespace s3Dive {

    class ModelLoadingSystem {
    public:
        void loadModel(Scene& scene, const std::string& filepath);

    private:
        MeshComponent processMesh(const aiMesh* mesh);
        MaterialComponent processMaterial(const aiMaterial* material) const;
        static std::shared_ptr<GLTexture> loadMaterialTexture(const aiMaterial* mat, aiTextureType type);
        static void initializeMeshComponent(MeshComponent& meshComponent);

        void processNode(Scene &scene,
                         const aiNode *node,
                         const aiScene *aiScene,
                         const UUID &modelEntityUUID,
                         const glm::mat4& parentTransform);
    };

} // namespace s3Dive

#endif //THREEDIVE_MESHLOADINGSYSTEM_H