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
#include "openGLRender/gl_context.h"
#include "openGLRender/gl_renderer.h"
#include "renderer/Camera.h"
#include <vector>
#include <cmath>


float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;


s3Dive::VectorialCameraConfig config(glm::vec3(0.0f, 0.0f, 3.0f));
s3Dive::Camera camera(config);

std::vector<float> generateDetailedGridVertices(float size, float step, float extensionSize = 0.5f, int fadeSteps = 5) {
    std::vector<float> vertices;
    int lineCount = static_cast<int>((2 * size) / step) + 1;

    auto addLine = [&](const glm::vec3 &start, const glm::vec3 &end, bool isMainLine) {
        float alpha = isMainLine ? 1.0f : 0.5f;
        float visibility = isMainLine ? 1.0f : 0.0f;
        glm::vec3 direction = glm::normalize(end - start);
        glm::vec3 extension = direction * (isMainLine ? extensionSize : extensionSize * 0.5f);

        auto addVertex = [&](const glm::vec3 &pos, float a) {
            vertices.insert(vertices.end(), {pos.x, pos.y, pos.z, 1.0f, 1.0f, 1.0f, a, visibility});
        };

        // Main line
        addVertex(start, alpha);
        addVertex(end, alpha);

        // Extensions
        for (int i = 0; i < fadeSteps; ++i) {
            float t1 = static_cast<float>(i) / fadeSteps;
            float t2 = static_cast<float>(i + 1) / fadeSteps;
            float alpha1 = alpha * (1.0f - t1);
            float alpha2 = alpha * (1.0f - t2);

            // Start extension
            addVertex(start - extension * t1, alpha1);
            addVertex(start - extension * t2, alpha2);

            // End extension
            addVertex(end + extension * t1, alpha1);
            addVertex(end + extension * t2, alpha2);
        }
    };

    // Generate lines
    for (int i = 0; i < lineCount; ++i) {
        float pos = -size + i * step;
        // Vertical lines
        addLine({pos, -size, 0}, {pos, size, 0}, true);
        // Horizontal lines
        addLine({-size, pos, 0}, {size, pos, 0}, true);

        // Detail lines
        if (i < lineCount - 1) {
            float detailPos = pos + step * 0.5f;
            addLine({detailPos, -size, 0}, {detailPos, size, 0}, false);
            addLine({-size, detailPos, 0}, {size, detailPos, 0}, false);
        }
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

void processInput(GLFWwindow *window, s3Dive::Camera &camera, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(s3Dive::CameraMovement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(s3Dive::CameraMovement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(s3Dive::CameraMovement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(s3Dive::CameraMovement::RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    static float lastX = 800.0f / 2.0;
    static float lastY = 600.0 / 2.0;

    static bool firstMouse = true;

    if (firstMouse) {
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

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
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

    float gridSize = 10.0f;
    float gridStep = 2.0f;
    float extensionSize = 4.0f;
    int fadeSteps = 5;
    float detailLineLength = 0.5f;
    auto sphere_vertices = generateNormalizedSphereVertices(36, 18);
    auto indices = generateSphereIndices(36, 18);
    auto grid_vertices = generateDetailedGridVertices(gridSize, gridStep, extensionSize, fadeSteps);

    s3Dive::GLVertexArray vao;
    auto vbo = std::make_shared<s3Dive::GLVertexBuffer>(grid_vertices);
//    auto ibo = std::make_shared<s3Dive::GLIndexBuffer>(indices);
    s3Dive::GLVertexBufferLayout layout;
    layout.addVertexElement<float>(3);
    layout.addVertexElement<float>(4);
    layout.addVertexElement<float>(1);
    vbo->setLayout(layout);

    vao.addVertexBuffer(vbo);
    //vao.setIndexBuffer(ibo);
    //s3Dive::GLTexture tex("wall.jpg");
    s3Dive::GLShaderProgram triangle_shader;
    triangle_shader.initFromFiles("grid.vert", "grid.frag");
    // Setup Dear ImGui

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
        renderer.drawLines(vao, grid_vertices.size() / 8);
        vao.bind();
        // feed inputs to dear imgui, start new frame
        glm::mat4 model = glm::mat4(1.0f);
        triangle_shader.updateShaderUniform("model", glm::value_ptr(model));
        glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), (float) SCR_WIDTH / (float) SCR_HEIGHT,
                                                0.1f, 100.0f);
        triangle_shader.updateShaderUniform("projection", glm::value_ptr(projection));
        glm::mat4 view = camera.getViewMatrix();
        triangle_shader.updateShaderUniform("view", glm::value_ptr(view));
        float cameraDistance = glm::length(camera.getZoom() - glm::vec3(0.0f, 0.0f, 0.0f));
        float detailVisibility = glm::clamp(1.0f - (cameraDistance / 40.0f), 0.0f, 1.0f);

        triangle_shader.updateShaderUniform("detailVisibility", detailVisibility);
        context.swapBuffers();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
