#include "App.h"
#include "InputManager.h"
#include <spdlog/spdlog.h>
#include <glm/gtc/type_ptr.hpp>
#include "geo_util.h"

namespace s3Dive {

    TestApplication::TestApplication(GLFWwindow* window) : window_(window) {}

    TestApplication::~TestApplication() = default;

    bool TestApplication::initialize() {
        inputManager_ = std::make_unique<InputManager>(window_);
        initializeCamera();
        initializeEventSystem();
        initializeGrid();

        glEnable(GL_DEPTH_TEST);
        int screenWidth, screenHeight;
        glfwGetFramebufferSize(window_, &screenWidth, &screenHeight);
        glViewport(0, 0, screenWidth, screenHeight);

        gridShader_.initFromFiles("grid.vert", "grid.frag");

        return true;
    }

    void TestApplication::run() {
        float lastFrame = 0.0f;
        while (!glfwWindowShouldClose(window_)) {
            float currentFrame = static_cast<float>(glfwGetTime());
            float deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            onUpdate(deltaTime);
            onRender();

            glfwSwapBuffers(window_);
            glfwPollEvents();
        }
    }

    void TestApplication::onUpdate(float deltaTime) {
        inputManager_->update();
        cameraController_->update(deltaTime);
        inputManager_->resetScrollDelta();
    }

    void TestApplication::onRender() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.34f, 0.34f, 0.34f, 1.0f);

        gridShader_.use();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

        glm::mat4 model(1.0f);
        gridShader_.updateShaderUniform("model", glm::value_ptr(model));

        const auto& camera = cameraController_->getCamera();
        glm::mat4 projection = camera.getProjectionMatrix();
        gridShader_.updateShaderUniform("projection", glm::value_ptr(projection));

        glm::mat4 view = camera.getViewMatrix();
        gridShader_.updateShaderUniform("view", glm::value_ptr(view));

        gridShader_.updateShaderUniform("detailVisibility1", cameraController_->getDetailVisibility1());
        gridShader_.updateShaderUniform("detailVisibility2", cameraController_->getDetailVisibility2());


        renderer_.drawLines(vao_, static_cast<GLint>(gridVertices_.size() / 8));
    }

    void TestApplication::initializeCamera() {
        auto camera = std::make_unique<PerspectiveCamera>(45.0f,
                                                          static_cast<float>(kScreenWidth)
                                                          / static_cast<float>(kScreenHeight),
                                                          0.1f,
                                                          100.0f);
        CameraSettings cameraSettings;
        cameraSettings.initialPosition = glm::vec3(40.0f, 40.0f, 40.0f);
        cameraSettings.targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
        cameraSettings.upVector = glm::vec3(0.0f, 0.0f, 1.0f);
        cameraController_ = std::make_unique<CameraController>(std::move(camera), *inputManager_);
    }

    void TestApplication::initializeEventSystem() {
        glfwSetWindowUserPointer(window_, this);
        glfwSetMouseButtonCallback(window_, mouseButtonCallback);
        glfwSetCursorPosCallback(window_, cursorPositionCallback);
        glfwSetScrollCallback(window_, scrollCallback);
        glfwSetKeyCallback(window_, keyCallback);
        glfwSetFramebufferSizeCallback(window_, windowResizeCallback);
    }

    void TestApplication::initializeGrid() {
        constexpr float gridSize = 10.0f;
        constexpr float gridStep = 2.0f;
        constexpr float extensionSize = 4.0f;
        constexpr int fadeSteps = 5;
        gridVertices_ = s3Dive::generateDetailedGridVertices(gridSize, gridStep, extensionSize, fadeSteps);

        vao_.bind();
        vbo_ = std::make_shared<GLVertexBuffer>(gridVertices_);
        GLVertexBufferLayout layout;
        layout.addVertexElement<float>(3);
        layout.addVertexElement<float>(4);
        layout.addVertexElement<float>(1);
        vbo_->setLayout(layout);

        vao_.addVertexBuffer(vbo_);
    }

    void TestApplication::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        auto* app = static_cast<TestApplication*>(glfwGetWindowUserPointer(window));
        app->inputManager_->mouseButtonCallback(window, button, action, mods);
    }

    void TestApplication::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
        auto* app = static_cast<TestApplication*>(glfwGetWindowUserPointer(window));
        app->inputManager_->cursorPosCallback(window, xpos, ypos);
    }

    void TestApplication::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        auto* app = static_cast<TestApplication*>(glfwGetWindowUserPointer(window));
        app->inputManager_->scrollCallback(window, xoffset, yoffset);
    }

    void TestApplication::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto* app = static_cast<TestApplication*>(glfwGetWindowUserPointer(window));
        // Implement key handling if needed
    }

    void TestApplication::windowResizeCallback(GLFWwindow* window, int width, int height) {
        auto* app = static_cast<TestApplication*>(glfwGetWindowUserPointer(window));
        glViewport(0, 0, width, height);
        app->cameraController_->getCamera().setAspectRatio(static_cast<float>(width) / static_cast<float>(height));
    }

} // namespace s3Dive

