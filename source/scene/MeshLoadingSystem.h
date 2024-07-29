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

    class MeshLoadingSystem : public System {
    public:
        void update(Scene& scene, float deltaTime) override;
        void loadModel(Scene& scene, const UUID& modelEntityUUID);
        static void render(Scene& scene, GLShaderProgram& shaderProgram);

    private:
        void processNode(Scene& scene, aiNode* node, const aiScene* aiScene, const UUID& modelEntityUUID, int depth = 0);
        MeshComponent processMesh(aiMesh* mesh, const aiScene* scene);
        static void clearExistingMeshes(Scene& scene, const UUID& modelEntityUUID);
        static void initialize(MeshComponent& meshComponent);
        static std::shared_ptr<GLTexture> loadMaterialTexture(const aiMaterial* mat, aiTextureType type);
    };

} // s3Dive

#endif //THREEDIVE_MESHLOADINGSYSTEM_H
