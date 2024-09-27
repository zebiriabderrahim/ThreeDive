#include "app.h"
#include <glm/gtc/type_ptr.hpp>
#include "../renderer/RenderCommand.h"
#include "geo_generator.h"
#include "window.h"
#include <spdlog/spdlog.h>

namespace s3Dive {

    App::App(const std::shared_ptr<Window>& window) : window_(window) {
        window_->addEventListener(EventType::MouseMoved, [this](const EventVariant& event) {
            cameraController_.onEvent(event);
        });
        window_->addEventListener(EventType::MouseButtonPressed, [this](const EventVariant& event) {
            cameraController_.onEvent(event);
        });
        window_->addEventListener(EventType::MouseButtonReleased, [this](const EventVariant& event) {
            cameraController_.onEvent(event);
        });
        window_->addEventListener(EventType::MouseScrolled, [this](const EventVariant& event) {
            cameraController_.onEvent(event);
        });
        window_->addEventListener(EventType::KeyPressed, [this](const EventVariant& event) {
            cameraController_.onEvent(event);
        });
        window_->addEventListener(EventType::WindowResize, [this](const EventVariant& event) {
            cameraController_.onEvent(event);
        });
    }

    App::~App() = default;

    bool App::initialize() {
        RenderCommand::init();
        initializeGrid();
        initializeSystems();


        glEnable(GL_DEPTH_TEST);

        gridShader_.initFromFiles("grid.vert", "grid.frag");
        defaultShaderProgram_.initFromFiles("simple-shader.vs.glsl", "simple-shader.fs.glsl");

        try {
            meshLoadingSystem_.loadModel(scene_, "obj.fbx");
                spdlog::info("Model added successfully with UUID: {}");
        } catch (const std::exception& e) {
            spdlog::error("Failed to add model: {}", e.what());
            // Handle the error appropriately
        }
        // Create default lights
        //createDefaultLights();


        return true;
    }

    void App::run() {
        while (!glfwWindowShouldClose(window_->getNativeWindow())) {
            window_->onUpdate();
            cameraController_.update();
            systems_.update(scene_, 0);
            defaultRenderSystem.update(scene_, 0);

            onRender();
        }
    }

    void App::onRender() {
        RenderCommand::clear(GLRenderer::BufferBit::Depth, GLRenderer::BufferBit::Color);
        RenderCommand::setClearColor(glm::vec4(0.266f, 0.26f, 0.25f, 1.0f));

        const auto& camera = cameraController_.getCamera();
        glm::mat4 projection = camera.getProjectionMatrix();
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 model(1.0f);

        // Set uniforms for grid shader
        gridShader_.use();
        gridShader_.updateShaderUniform("model", glm::value_ptr(model));
        gridShader_.updateShaderUniform("view", glm::value_ptr(view));
        gridShader_.updateShaderUniform("projection", glm::value_ptr(projection));

        // Render grid
        systems_.render(scene_, gridShader_, cameraController_);

        defaultRenderSystem.render(scene_, defaultShaderProgram_, cameraController_);
    }


    void App::initializeGrid() {
        auto gridEntity = scene_.createEntity();
        scene_.addComponent<SceneGridComponent>(scene_.getEntityUUID(gridEntity).value());

    }


    void App::initializeSystems() {
        //systems_.push_back(std::make_unique<SceneGridSystem>());
        // Add other systems as needed
    }

    void App::createDefaultLights() {
        // Key light
        UUID keyLightUUID{};
        LightComponent keyLight;
        keyLight.type = LightType::Directional;
        keyLight.direction = glm::normalize(glm::vec3(1.0f, -1.0f, -1.0f));
        keyLight.color = glm::vec3(1.0f, 1.0f, 0.9f);
        keyLight.intensity = 1.0f;
        scene_.addComponent<LightComponent>(keyLightUUID, keyLight);

        // Fill light
        UUID fillLightUUID{};
        LightComponent fillLight;
        fillLight.type = LightType::Directional;
        fillLight.direction = glm::normalize(glm::vec3(-1.0f, -0.5f, 0.5f));
        fillLight.color = glm::vec3(0.7f, 0.7f, 0.8f);
        fillLight.intensity = 0.5f;
        scene_.addComponent<LightComponent>(fillLightUUID, fillLight);

        // Back light
        UUID backLightUUID{};
        LightComponent backLight;
        backLight.type = LightType::Directional;
        backLight.direction = glm::normalize(glm::vec3(0.0f, 1.0f, 1.0f));
        backLight.color = glm::vec3(0.9f, 0.9f, 1.0f);
        backLight.intensity = 0.3f;
        scene_.addComponent<LightComponent>(backLightUUID, backLight);
    }


} // namespace s3Dive
