//
// Created by ABDERRAHIM ZEBIRI on 2024-07-14.
//
#pragma once

#ifndef THREEDIVE_MODEL_H
#define THREEDIVE_MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <string>
#include <memory>
#include "../platform/openGLRender/gl_vertex_array.h"
#include "../platform/openGLRender/gl_vertex_buffer.h"
#include "../platform/openGLRender/gl_index_buffer.h"
#include "../platform/openGLRender/gl_texture.h"
#include "../platform/openGLRender/gl_shader_program.h"

namespace s3Dive {

    class Model {
        public:
            Model();
            ~Model() = default;

            bool LoadModel(const std::string& path);
            void Draw(GLShaderProgram& shader) const;

        private:
            struct Mesh {
                std::unique_ptr<GLVertexArray> vao;
                std::shared_ptr<GLVertexBuffer> vbo;
                std::unique_ptr<GLIndexBuffer> ibo;
                std::vector<std::shared_ptr<GLTexture>> textures;
                unsigned int indexCount;
            };

            void ProcessNode(aiNode* node, const aiScene* scene);
            Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
            std::vector<std::shared_ptr<GLTexture>> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);

            std::vector<Mesh> m_Meshes;
            std::string m_Directory;
            std::vector<std::shared_ptr<GLTexture>> m_LoadedTextures;
        };

} // s3Dive

#endif //THREEDIVE_MODEL_H
