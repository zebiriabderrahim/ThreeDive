//
// Created by ABDERRAHIM ZEBIRI on 2024-07-10.
//

#ifndef THREEDIVE_VIEWCUBE_H
#define THREEDIVE_VIEWCUBE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include <string>
#include <iostream>

#include "../openGLRender/gl_vertex_buffer_layout.h"
#include "../openGLRender/gl_vertex_buffer.h"
#include "../openGLRender/gl_index_buffer.h"
#include "../openGLRender/gl_vertex_array.h"
#include "../openGLRender/gl_shader.h"



namespace s3Dive {



    class ViewCube {
    public:
        ViewCube(int size, const std::string& fontPath);
        ~ViewCube();

        void render(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix);

    private:
        struct Character {
            GLuint TextureID;
            glm::ivec2 Size;
            glm::ivec2 Bearing;
            GLuint Advance;
        };

        int size;
        GLVertexArray cubeVAO;
        GLVertexBuffer cubeVBO;
        GLIndexBuffer cubeEBO;
        GLShader cubeShader;
        GLVertexArray  textVAO;
        GLVertexBuffer textVBO;
        GLIndexBuffer  textShader;
        std::map<GLchar, Character> Characters;

        void initializeCubeGeometry();
        void initializeCubeShaders();
        void initializeTextRendering(const std::string& fontPath);
        void initializeTextShaders();
        void drawLabel(const std::string& text, const glm::vec3& position, const glm::vec3& color);
        GLuint compileShader(GLenum type, const char* source);
        GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource);
    };

} // s3Dive

#endif //THREEDIVE_VIEWCUBE_H
