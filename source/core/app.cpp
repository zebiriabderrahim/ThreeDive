#include "app.h"
#include <glm/gtc/type_ptr.hpp>
#include "../renderer/RenderCommand.h"
#include "geo_generator.h"
#include "window.h"

namespace s3Dive {

    App::App(const std::shared_ptr<Window>& window) : window_(window) {
        window_->addEventListener(EventType::MouseMoved, [this](const EventVariant& event) {
            cameraController_->onEvent(event);
        });
        window_->addEventListener(EventType::MouseButtonPressed, [this](const EventVariant& event) {
            cameraController_->onEvent(event);
        });
        window_->addEventListener(EventType::MouseButtonReleased, [this](const EventVariant& event) {
            cameraController_->onEvent(event);
        });
        window_->addEventListener(EventType::MouseScrolled, [this](const EventVariant& event) {
            cameraController_->onEvent(event);
        });
        window_->addEventListener(EventType::KeyPressed, [this](const EventVariant& event) {
            cameraController_->onEvent(event);
        });
        window_->addEventListener(EventType::WindowResize, [this](const EventVariant& event) {
            cameraController_->onEvent(event);
        });
    }

    App::~App() = default;

    bool App::initialize() {
        RenderCommand::init();
        initializeCamera();
        initializeGrid();
        initializeSystems();

        glEnable(GL_DEPTH_TEST);

        gridShader_.initFromFiles("grid.vert", "grid.frag");

        return true;
    }

    void App::run() {
        while (!glfwWindowShouldClose(window_->getNativeWindow())) {
            window_->onUpdate();
            cameraController_->update();

//            // Update systems
//            for (auto& system : systems_) {
//                system->update(scene_, window_->getDeltaTime());
//            }
            systems_.update(scene_, 0);

            onRender();
        }
    }

    void App::onRender() {
        RenderCommand::clear(GLRenderer::BufferBit::Depth, GLRenderer::BufferBit::Color);
        RenderCommand::setClearColor(glm::vec4(0.266f, 0.26f, 0.25f, 1.0f));

        gridShader_.use();

        glm::mat4 model(1.0f);
        gridShader_.updateShaderUniform("model", glm::value_ptr(model));

        const auto& camera = cameraController_->getCamera();
        glm::mat4 projection = camera.getProjectionMatrix();
        gridShader_.updateShaderUniform("projection", glm::value_ptr(projection));

        glm::mat4 view = camera.getViewMatrix();
        gridShader_.updateShaderUniform("view", glm::value_ptr(view));

        // Render systems
//        for (auto& system : systems_) {
//            system->render(scene_, gridShader_, *cameraController_);
//        }
        systems_.render(scene_, gridShader_, *cameraController_);

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

        // Create grid entity in the scene
        auto gridEntity = scene_.createEntity();
        scene_.addComponent<SceneGridComponent>(scene_.getEntityUUID(gridEntity).value(),
                                                gridSize, gridStep, extensionSize, fadeSteps);

    }


    void App::initializeSystems() {
        //systems_.push_back(std::make_unique<SceneGridSystem>());
        // Add other systems as needed
    }

} // namespace s3Dive
