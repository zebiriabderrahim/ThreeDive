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
        void loadModel(Scene& scene, const UUID& modelEntityUUID);

    private:
        void processNode(Scene& scene, const aiNode* node, const aiScene* aiScene, const UUID& modelEntityUUID, int depth = 0);
        MeshComponent processMesh(const aiMesh* mesh, const aiScene* scene);
        MaterialComponent processMaterial(const aiMaterial* material);
        static std::shared_ptr<GLTexture> loadMaterialTexture(const aiMaterial* mat, aiTextureType type);
    };

} // s3Dive

#endif //THREEDIVE_MESHLOADINGSYSTEM_H