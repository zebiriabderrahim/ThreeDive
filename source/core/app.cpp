#include "app.h"
#include "InputManager.h"
#include <glm/gtc/type_ptr.hpp>
#include "geo_util.h"
#include "../renderer/RenderCommand.h"

namespace s3Dive {

    App::App(const std::shared_ptr<Window> &window) : window_(window) {

        cameraController_ = std::make_unique<CameraController>(
                std::make_unique<PerspectiveCamera>(45.0f, static_cast<float>(kScreenWidth)
                                                           / static_cast<float>(kScreenHeight),
                                                    0.1f, 100.0f),
                CameraSettings{});

        window_->addEventListener(EventType::MouseMoved, [this](const EventVariant &event) {
            cameraController_->onEvent(event);
        });
        window_->addEventListener(EventType::MouseButtonPressed, [this](const EventVariant &event) {
            cameraController_->onEvent(event);
        });
        window_->addEventListener(EventType::MouseButtonReleased, [this](const EventVariant &event) {
            cameraController_->onEvent(event);
        });
        window_->addEventListener(EventType::MouseScrolled, [this](const EventVariant &event) {
            cameraController_->onEvent(event);
        });
        window_->addEventListener(EventType::WindowResize, [this](const EventVariant &event) {
            cameraController_->onEvent(event);
        });

    }

    App::~App() = default;

    bool App::initialize() {
        RenderCommand::init();
        initializeCamera();
        initializeGrid();

        glEnable(GL_DEPTH_TEST);

        gridShader_.initFromFiles("grid.vert", "grid.frag");

        return true;
    }

    void App::run() {
        float lastFrame = 0.0f;
        while (!glfwWindowShouldClose(window_.get()->getNativeWindow())) {
            auto currentFrame = static_cast<float>(glfwGetTime());
            float deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            window_->onUpdate();
            cameraController_->update();
            onRender();

        }
    }

    void App::onRender() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.266f, 0.26f, 0.25f, 1.0f);

        gridShader_.use();


        glm::mat4 model(1.0f);
        gridShader_.updateShaderUniform("model", glm::value_ptr(model));

        const auto &camera = cameraController_->getCamera();
        glm::mat4 projection = camera.getProjectionMatrix();
        gridShader_.updateShaderUniform("projection", glm::value_ptr(projection));

        glm::mat4 view = camera.getViewMatrix();
        gridShader_.updateShaderUniform("view", glm::value_ptr(view));

        gridShader_.updateShaderUniform("detailVisibility1", cameraController_->getDetailVisibility1());
        gridShader_.updateShaderUniform("detailVisibility2", cameraController_->getDetailVisibility2());


        RenderCommand::drawLines(vao_, static_cast<GLint>(gridVertices_.size() / 8));
    }

    void App::initializeCamera() {
        auto camera = std::make_unique<PerspectiveCamera>(45.0f,
                                                          static_cast<float>(kScreenWidth)
                                                          / static_cast<float>(kScreenHeight),
                                                          0.1f,
                                                          100.0f);
        CameraSettings cameraSettings;
        cameraSettings.initialPosition = glm::vec3(40.0f, 40.0f, 40.0f);
        cameraSettings.targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
        cameraSettings.upVector = glm::vec3(0.0f, 0.0f, 1.0f);
        cameraController_ = std::make_unique<CameraController>(std::move(camera));
    }


    void App::initializeGrid() {
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


} // namespace s3Dive

