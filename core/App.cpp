#include "App.h"

#include <spdlog/spdlog.h>
#include <glm/gtc/type_ptr.hpp>

namespace s3Dive {

    TestApplication::TestApplication(GLFWwindow* window) : window_(window) {}

    TestApplication::~TestApplication() = default;

    bool TestApplication::initialize() {
        initializeCamera();
        initializeEventSystem();
        initializeGrid();

        glEnable(GL_DEPTH_TEST);
        int screen_width, screen_height;
        glfwGetFramebufferSize(window_, &screen_width, &screen_height);
        glViewport(0, 0, screen_width, screen_height);

        triangleShader_.initFromFiles("grid.vert", "grid.frag");

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
        processInput(deltaTime);
        eventQueue_.processEvents(eventDispatcher_);
    }

    void TestApplication::onRender() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.34f, 0.34f, 0.34f, 1.0f);

        triangleShader_.use();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

        glm::mat4 model = glm::mat4(1.0f);
        triangleShader_.updateShaderUniform("model", glm::value_ptr(model));

        glm::mat4 projection = cameraController_->getCamera().getProjectionMatrix();
        triangleShader_.updateShaderUniform("projection", glm::value_ptr(projection));

        glm::mat4 view = cameraController_->getCamera().getViewMatrix();
        triangleShader_.updateShaderUniform("view", glm::value_ptr(view));

        float cameraDistance = glm::length(cameraController_->getCamera().getPosition() - glm::vec3(0.0f, 0.0f, 0.0f));
        float detailVisibility = glm::clamp(1.0f - (cameraDistance / 40.0f), 0.0f, 1.0f);
        triangleShader_.updateShaderUniform("detailVisibility", detailVisibility);

        vao_.bind();
        glDrawArrays(GL_LINES, 0, gridVertices_.size() / 8);
    }

    void TestApplication::initializeCamera() {
        camera_ = std::make_unique<PerspectiveCamera>(45.0f, static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 100.0f);
        camera_->setPosition(glm::vec3(0.0f, 0.0f, 3.0f));
        cameraController_ = std::make_unique<CameraController>(std::move(camera_), window_);
    }

    void TestApplication::initializeEventSystem() {
        glfwSetWindowUserPointer(window_, this);
        glfwSetMouseButtonCallback(window_, mouseButtonCallback);
        glfwSetCursorPosCallback(window_, cursorPositionCallback);
        glfwSetScrollCallback(window_, scrollCallback);
        glfwSetKeyCallback(window_, keyCallback);

        eventDispatcher_.addEventListener(EventType::MouseMoved, [this](const EventVariant& event) {
            cameraController_->handleEvent(event);
        });
        eventDispatcher_.addEventListener(EventType::MouseScrolled, [this](const EventVariant& event) {
            cameraController_->handleEvent(event);
        });
        eventDispatcher_.addEventListener(EventType::MouseButtonPressed, [this](const EventVariant& event) {
            cameraController_->handleEvent(event);
        });
        eventDispatcher_.addEventListener(EventType::MouseButtonReleased, [this](const EventVariant& event) {
            cameraController_->handleEvent(event);
        });
        eventDispatcher_.addEventListener(EventType::KeyPressed, [this](const EventVariant& event) {
            cameraController_->handleEvent(event);
        });
        eventDispatcher_.addEventListener(EventType::KeyReleased, [this](const EventVariant& event) {
            cameraController_->handleEvent(event);
        });
    }

    void TestApplication::initializeGrid() {
        float gridSize = 10.0f;
        float gridStep = 2.0f;
        float extensionSize = 4.0f;
        int fadeSteps = 5;
        gridVertices_ = generateDetailedGridVertices(gridSize, gridStep, extensionSize, fadeSteps);

        vao_.bind();
        vbo_ = std::make_shared<GLVertexBuffer>(gridVertices_);
        GLVertexBufferLayout layout;
        layout.addVertexElement<float>(3);
        layout.addVertexElement<float>(4);
        layout.addVertexElement<float>(1);
        vbo_->setLayout(layout);

        vao_.addVertexBuffer(vbo_);
    }

    void TestApplication::processInput(float deltaTime) {
        if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window_, true);
    }

    std::vector<float> TestApplication::generateDetailedGridVertices(float size, float step, float extensionSize, int fadeSteps) {
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

    void TestApplication::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        auto* app = static_cast<TestApplication*>(glfwGetWindowUserPointer(window));
        if (action == GLFW_PRESS) {
            app->eventQueue_.enqueueEvent(MouseButtonPressedEvent{static_cast<MouseCode>(button)});
        } else if (action == GLFW_RELEASE) {
            app->eventQueue_.enqueueEvent(MouseButtonReleasedEvent{static_cast<MouseCode>(button)});
        }
    }

    void TestApplication::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
        auto* app = static_cast<TestApplication*>(glfwGetWindowUserPointer(window));
        app->eventQueue_.enqueueEvent(MouseMovedEvent{static_cast<float>(xpos), static_cast<float>(ypos)});
    }

    void TestApplication::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        auto* app = static_cast<TestApplication*>(glfwGetWindowUserPointer(window));
        app->eventQueue_.enqueueEvent(MouseScrolledEvent{static_cast<float>(xoffset), static_cast<float>(yoffset)});
    }

    void TestApplication::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto* app = static_cast<TestApplication*>(glfwGetWindowUserPointer(window));
        if (action == GLFW_PRESS) {
            app->eventQueue_.enqueueEvent(KeyPressedEvent{static_cast<Key::KeyCode>(key), false});
        } else if (action == GLFW_RELEASE) {
            app->eventQueue_.enqueueEvent(KeyReleasedEvent{static_cast<Key::KeyCode>(key)});
        }
    }

} // namespace s3Dive

