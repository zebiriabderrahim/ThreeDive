#include "app.h"
#include <glm/gtc/type_ptr.hpp>
#include "../renderer/RenderCommand.h"
#include "geo_generator.h"

namespace s3Dive {

    App::App(const std::shared_ptr<Window> &window) : window_(window) {

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
        window_->addEventListener(EventType::KeyPressed, [this](const EventVariant &event) {
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

        while (!glfwWindowShouldClose(window_.get()->getNativeWindow())) {

            window_->onUpdate();
            cameraController_->update();
            onRender();

        }
    }

    void App::onRender() {
        RenderCommand::clear(GLRenderer::BufferBit::Depth, GLRenderer::BufferBit::Color);
        RenderCommand::setClearColor(glm::vec4(0.266f, 0.26f, 0.25f, 1.0f));

        gridShader_.use();


        glm::mat4 model(1.0f);
        gridShader_.updateShaderUniform("model", glm::value_ptr(model));

        const auto &camera = cameraController_->getCamera();
        glm::mat4 projection = camera.getProjectionMatrix();
        gridShader_.updateShaderUniform("projection", glm::value_ptr(projection));

        glm::mat4 view = camera.getViewMatrix();
        gridShader_.updateShaderUniform("view", glm::value_ptr(view));

        gridShader_.updateShaderUniform("detailVisibility1", cameraController_->getDistanceToTarget());
        gridShader_.updateShaderUniform("detailVisibility2", cameraController_->getDistanceToTarget() - 1.0f);


        RenderCommand::drawLines(vao_, static_cast<GLint>(gridVertices_.size() / 8));
    }

    void App::initializeCamera() {
        CameraSettings cameraSettings;
        cameraSettings.initialPosition = glm::vec3(40.0f, 40.0f, 40.0f);
        cameraSettings.targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
        cameraSettings.upVector = glm::vec3(0.0f, 0.0f, 1.0f);
        cameraController_ = std::make_unique<CameraController>(cameraSettings);
    }


    void App::initializeGrid() {
        constexpr float gridSize = 10.0f;
        constexpr float gridStep = 2.0f;
        constexpr float extensionSize = 4.0f;
        constexpr int fadeSteps = 5;
        gridVertices_ = geo::generateDetailedGridVertices(gridSize, gridStep, extensionSize, fadeSteps);

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

