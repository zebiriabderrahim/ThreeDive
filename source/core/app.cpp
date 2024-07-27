#include "app.h"
#include <glm/gtc/type_ptr.hpp>
#include "../renderer/RenderCommand.h"
#include "geo_generator.h"
#include "window.h"

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
        shader_.initFromFiles("simple-shader.vs.glsl", "simple-shader.fs.glsl");

        UUID modelEntityUUID {};
        scene_.addComponent<ModelComponent>(modelEntityUUID, ModelComponent{"OS.obj", {}});
        meshLoadingSystem_.loadModel(scene_, modelEntityUUID);

        return true;
    }

    void App::run() {
        while (!glfwWindowShouldClose(window_->getNativeWindow())) {
            window_->onUpdate();
            cameraController_.update();

//            // Update systems
//            for (auto& system : systems_) {
//                system->update(scene_, window_->getDeltaTime());
//            }
            systems_.update(scene_, 0);
            meshLoadingSystem_.update(scene_, 0);

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

        // Set uniforms for model shader
        shader_.use();
        shader_.updateShaderUniform("model", glm::value_ptr(model));
        shader_.updateShaderUniform("view", glm::value_ptr(view));
        shader_.updateShaderUniform("projection", glm::value_ptr(projection));

        // Set default material properties
        shader_.updateShaderUniform("objectColor", glm::vec3(0.8f, 0.8f, 0.8f));
        shader_.updateShaderUniform("specularStrength", 0.5f);

        // Set lighting properties
        shader_.updateShaderUniform("lightPos", glm::vec3(10.0f, 10.0f, 10.0f));
        shader_.updateShaderUniform("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

        // Set other uniforms
        shader_.updateShaderUniform("useVertexColor", false); // Set to true if using vertex colors
        shader_.updateShaderUniform("isWireframe", false); // Set to true for wireframe mode


        // Render models
        s3Dive::MeshLoadingSystem::render(scene_, shader_);
    }


    void App::initializeGrid() {
        auto gridEntity = scene_.createEntity();
        scene_.addComponent<SceneGridComponent>(scene_.getEntityUUID(gridEntity).value());

    }


    void App::initializeSystems() {
        //systems_.push_back(std::make_unique<SceneGridSystem>());
        // Add other systems as needed
    }

} // namespace s3Dive
