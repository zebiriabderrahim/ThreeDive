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
#include "renderer/ortho_camera.h"
#include "renderer/Camera.h"
#include <vector>
#include <cmath>
bool firstMouse = true;
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
s3Dive::VectorialCameraConfig config(glm::vec3(0.0f, 0.0f, 3.0f));
s3Dive::Camera camera(config);
std::vector<float> generateDetailedGridVertices(float size, float step, float extensionSize = 0.5f, int fadeSteps = 5) {
    std::vector<float> vertices;

    int mainLineCount = static_cast<int>((2 * size) / step) + 1;
    float stepAlpha = 1.0f / (fadeSteps + 1);

    auto addLine = [&](float x1, float y1, float x2, float y2, bool isMainLine) {
        float alpha = isMainLine ? 1.0f : 0.5f;
        float visibility = isMainLine ? 1.0f : 0.0f; // 1.0 for main lines, 0.0 for detail lines
        float lineExtensionSize = isMainLine ? extensionSize : extensionSize * 0.5f;

        // Main part of the line
        vertices.insert(vertices.end(), {
                x1, y1, 0.0f, 1.0f, 1.0f, 1.0f, alpha, visibility
        });
        vertices.insert(vertices.end(), {
                x2, y2, 0.0f, 1.0f, 1.0f, 1.0f, alpha, visibility
        });

        // Fading extensions
        for (int j = 1; j <= fadeSteps; ++j) {
            float fadeAlpha = alpha * (1.0f - j * stepAlpha);
            float extLength = j * (lineExtensionSize / fadeSteps);

            if (x1 == x2) { // Vertical line
                vertices.insert(vertices.end(), {
                        x1, y1 - extLength, 0.0f, 1.0f, 1.0f, 1.0f, fadeAlpha, visibility,
                        x1, y1 - extLength + (lineExtensionSize / fadeSteps), 0.0f, 1.0f, 1.0f, 1.0f, fadeAlpha - stepAlpha, visibility,
                        x2, y2 + extLength - (lineExtensionSize / fadeSteps), 0.0f, 1.0f, 1.0f, 1.0f, fadeAlpha, visibility,
                        x2, y2 + extLength, 0.0f, 1.0f, 1.0f, 1.0f, fadeAlpha - stepAlpha, visibility
                });
            } else { // Horizontal line
                vertices.insert(vertices.end(), {
                        x1 - extLength, y1, 0.0f, 1.0f, 1.0f, 1.0f, fadeAlpha, visibility,
                        x1 - extLength + (lineExtensionSize / fadeSteps), y1, 0.0f, 1.0f, 1.0f, 1.0f, fadeAlpha - stepAlpha, visibility,
                        x2 + extLength - (lineExtensionSize / fadeSteps), y2, 0.0f, 1.0f, 1.0f, 1.0f, fadeAlpha, visibility,
                        x2 + extLength, y2, 0.0f, 1.0f, 1.0f, 1.0f, fadeAlpha - stepAlpha, visibility
                });
            }
        }
    };

    // Main grid lines
    for (int i = 0; i < mainLineCount; ++i) {
        float pos = -size + i * step;

        // Vertical main line
        addLine(pos, -size, pos, size, true);

        // Horizontal main line
        addLine(-size, pos, size, pos, true);
    }

    // Detail grid lines
    for (int i = 0; i < mainLineCount - 1; ++i) {
        float pos = -size + (i + 0.5f) * step;

        // Vertical detail line
        addLine(pos, -size, pos, size, false);

        // Horizontal detail line
        addLine(-size, pos, size, pos, false);
    }

    return vertices;
}





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
void processInput(GLFWwindow *window, s3Dive::Camera &camera, float deltaTime)
{
if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(s3Dive::CameraMovement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(s3Dive::CameraMovement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(s3Dive::CameraMovement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(s3Dive::CameraMovement::RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    static float lastX = 800.0f / 2.0;
    static float lastY = 600.0 / 2.0;

    static bool firstMouse = true;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;


    camera.processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.processMouseScroll(static_cast<float >(yoffset));
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
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Dear ImGui - Conan", nullptr, nullptr);
    if (window == nullptr) {
        spdlog::error("Failed to create GLFW window");
        return 1;
    }



    s3Dive::GLContext context(window);
    context.init();
    glEnable(GL_DEPTH_TEST);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    int screen_width;
    int screen_height;
    glfwGetFramebufferSize(window, &screen_width, &screen_height);
    renderer.setViewport(0, 0, screen_width, screen_height);

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
    float gridSize = 5.0f;
    float gridStep = 1.0f;
    float extensionSize = 0.5f;
    int fadeSteps = 5;

    // Calculate total number of vertices
    int mainLineCount = static_cast<int>((2 * gridSize) / gridStep) + 1;
    int verticesPerLine = 2 + 4 * fadeSteps; // 2 for main line, 4 for each fade step
    int totalVertices = mainLineCount * 2 * verticesPerLine; // Vertical and horizontal lines
    totalVertices += (mainLineCount - 1) * 2 * verticesPerLine; // Detail lines

    s3Dive::GLVertexArray vao;
    //auto vbo = std::make_shared<s3Dive::GLVertexBuffer>(grid_vertices);
//    auto ibo = std::make_shared<s3Dive::GLIndexBuffer>(indices);
    s3Dive::GLVertexBufferLayout layout;
    layout.addVertexElement<float>(3);
    layout.addVertexElement<float>(4);
    layout.addVertexElement<float>(1);
    //vbo->setLayout(layout);

    //vao.addVertexBuffer(vbo);
    //vao.setIndexBuffer(ibo);
    //s3Dive::GLTexture tex("wall.jpg");
    s3Dive::GLShaderProgram triangle_shader;
    triangle_shader.initFromFiles("grid.vert", "grid.frag");
   // Setup Dear ImGui



    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    [[maybe_unused]] ImGuiIO const &io = ImGui::GetIO();
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // disable the window resizing
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        renderer.clear();
        renderer.setClearColor(glm::vec4(glm::vec3(0.34f), 1.0f));
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window, camera, deltaTime);
        triangle_shader.use();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        renderer.setLineWidth(0.5f);
        glDrawArrays(GL_LINES, 0, totalVertices);
        vao.bind();
        // feed inputs to dear imgui, start new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        glm::mat4 model = glm::mat4(1.0f);
        triangle_shader.updateShaderUniform("model", glm::value_ptr(model));
        glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        triangle_shader.updateShaderUniform("projection", glm::value_ptr(projection));
        glm::mat4 view = camera.getViewMatrix();
        triangle_shader.updateShaderUniform("view", glm::value_ptr(view));
        float cameraDistance = glm::length(camera.getPosition() - glm::vec3(0.0f, 0.0f, 0.0f));
        float detailVisibility = glm::clamp(1.0f - (cameraDistance / 20.0f), 0.0f, 1.0f);

        triangle_shader.updateShaderUniform("detailVisibility", detailVisibility);

        ImGui::Begin("Triangle Position/Color");
        static bool wireframe = false;
        ImGui::Checkbox("Wireframe", &wireframe);
        glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
        ImGui::End();
        // Render dear imgui into screen
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
