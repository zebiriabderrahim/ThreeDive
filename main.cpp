#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "bindings/imgui_impl_glfw.h"
#include "bindings/imgui_impl_opengl3.h"
#include "openGLRender/gl_shader_program.h"
#include "openGLRender/gl_vertex_array.h"
#include "openGLRender/gl_vertex_buffer.h"
#include "openGLRender/gl_index_buffer.h"
#include "openGLRender/gl_vertex_buffer_layout.h"
#include "openGLRender/gl_texture.h"
#include "openGLRender/gl_context.h"
#include "openGLRender/gl_renderer.h"
#include <vector>
#include <cmath>

std::vector<float> generateNormalizedSphereVertices(int sectorCount, int stackCount) {
    std::vector<float> vertices;
    float radius = 1.0f; // Unit sphere

    for (int i = 0; i <= stackCount; ++i) {
        float stackAngle = glm::pi<float>() / 2 - i * glm::pi<float>() / stackCount; // from pi/2 to -pi/2
        float xy = radius * cosf(stackAngle);             // r * cos(u)
        float z = radius * sinf(stackAngle);              // r * sin(u)

        for (int j = 0; j <= sectorCount; ++j) {
            float sectorAngle = j * 2 * glm::pi<float>() / sectorCount; // from 0 to 2pi

            float x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            float y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // Texture coordinates
            float s = (float) j / sectorCount;
            float t = (float) i / stackCount;
            vertices.push_back(s);
            vertices.push_back(t);
        }
    }

    return vertices;
}

// Function to generate sphere indices
std::vector<unsigned int> generateSphereIndices(int sectorCount, int stackCount) {
    std::vector<unsigned int> indices;
    for (int i = 0; i < stackCount; ++i) {
        int k1 = i * (sectorCount + 1);     // beginning of current stack
        int k2 = k1 + sectorCount + 1;      // beginning of next stack

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stackCount - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    return indices;
}


int main(int, char **) {
    const char *glsl_version = "#version 150";
    s3Dive::GLRenderer renderer;
    renderer.init();
    // Create window with graphics context
    GLFWwindow *window = glfwCreateWindow(1280, 720, "Dear ImGui - Conan", nullptr, nullptr);
    if (window == nullptr) {
        spdlog::error("Failed to create GLFW window");
        return 1;
    }
    s3Dive::GLContext context(window);
    context.init();
    glEnable(GL_DEPTH_TEST);

    int screen_width;
    int screen_height;
    glfwGetFramebufferSize(window, &screen_width, &screen_height);
    renderer.setviewport(0, 0, screen_width, screen_height);

    // create our geometries
//    std::vector<float> triangle_vertices = {
//            // positions        // colors         // texture coords
//            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
//            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
//            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
//            -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
//    };
    std::vector<glm::vec3> cubePositions = {
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(2.0f, 5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3(2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f, 3.0f, -7.5f),
            glm::vec3(1.3f, -2.0f, -2.5f),
            glm::vec3(1.5f, 2.0f, -2.5f),
            glm::vec3(1.5f, 0.2f, -1.5f),
            glm::vec3(-1.3f, 1.0f, -1.5f)
    };


    std::vector<float> triangle_vertices = {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
    };


    auto sphere_vertices = generateNormalizedSphereVertices(36, 18);
    auto indices = generateSphereIndices(36, 18);
    s3Dive::GLVertexArray vao;
    auto vbo = std::make_shared<s3Dive::GLVertexBuffer>(sphere_vertices);
    auto ibo = std::make_shared<s3Dive::GLIndexBuffer>(indices);
    s3Dive::GLVertexBufferLayout layout;
    layout.addVertexElement<float>(3);
//    layout.addVertexElement<float>(3);
    layout.addVertexElement<float>(2);
    vbo->setLayout(layout);

    vao.addVertexBuffer(vbo);
    vao.setIndexBuffer(ibo);
    // texture .jpg is in the root you need to back for cmake-build-debug

    s3Dive::GLTexture tex("wall.jpg");



    // init shader
    s3Dive::GLShaderProgram triangle_shader;
    triangle_shader.initFromFiles("simple-shader.vs.glsl", "simple-shader.fs.glsl");
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    [[maybe_unused]] ImGuiIO const &io = ImGui::GetIO();
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        renderer.clear();
        renderer.setClearColor(glm::vec4(glm::vec3(0.24f), 1.0f));

        // feed inputs to dear imgui, start new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // rendering our geometries
        triangle_shader.use();
        tex.bind();
        vao.bind();
        renderer.drawIndexed(vao, indices.size());
        vao.unbind();

        ImGui::Begin("Triangle Position/Color");
        //check box to show wireframe
        static bool wireframe = false;
        ImGui::Checkbox("Wireframe", &wireframe);
        glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
        static float rotation = 0.0;
        ImGui::SliderFloat("rotation", &rotation, 0, 2 * glm::pi<float>());
        static float translation[] = {0.0, 0.0};
        ImGui::SliderFloat2("position", translation, -1.0, 1.0);
        static float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        // pass the parameters to the shader
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(translation[0], translation[1], 0.0f));
        model = glm::rotate(model, rotation, glm::vec3(0.0f, 1.0f, 1.0f));
        triangle_shader.updateShaderUniform("model", glm::value_ptr(model));

        ImGui::End();


        glm::mat4 view = glm::mat4(1.0f);
        // note that we’re translating the scene in the reverse direction
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), (float) screen_width / (float) screen_height,
                                      0.1f, 100.0f);

        triangle_shader.updateShaderUniform("view", glm::value_ptr(view));
        triangle_shader.updateShaderUniform("projection", glm::value_ptr(projection));
        // render your GUI

        // Render dear imgui into screen
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        int display_w;
        int display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        renderer.setviewport(0, 0, display_w, display_h);
        context.swapBuffers();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
