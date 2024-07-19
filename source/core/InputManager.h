//
// Created by ABDERRAHIM ZEBIRI on 2024-07-07.
//
#pragma once

#ifndef THREEDIVE_INPUTMANAGER_H
#define THREEDIVE_INPUTMANAGER_H


#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../events/key_codes.h"


namespace s3Dive {
    class InputManager {
    public:
        explicit InputManager(GLFWwindow* window);
        ~InputManager();

        InputManager(const InputManager&) = delete;
        InputManager& operator=(const InputManager&) = delete;

        void update();

        [[nodiscard]] bool isLeftMouseButtonPressed() const { return isLeftMouseButtonPressed_; }
        [[nodiscard]] bool isMiddleMouseButtonPressed() const { return isMiddleMouseButtonPressed_; }
        [[nodiscard]] bool isRightMouseButtonPressed() const { return isRightMouseButtonPressed_; }
        [[nodiscard]] const glm::vec2& getMousePosition() const { return mousePosition_; }
        [[nodiscard]] const glm::vec2& getMouseDelta() const { return mouseDelta_; }
        [[nodiscard]] float getScrollDelta() const { return scrollDelta_; }

        void resetMouseDelta() { mouseDelta_ = glm::vec2(0.0f); }
        void resetScrollDelta() { scrollDelta_ = 0.0f; }

        // Added methods
        [[nodiscard]] bool isKeyPressed(Key::KeyCode key) const;
        void setMousePosition(double xPos, double yPos);
        void setMouseButton(int button, bool pressed);
        void addScrollDelta(float delta);

        // Callback methods (to be called from GLFW callbacks)
        void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        void cursorPosCallback(GLFWwindow* window, double xPos, double yPos);
        void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
        void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    private:
        GLFWwindow* window_;
        bool isLeftMouseButtonPressed_ = false;
        bool isMiddleMouseButtonPressed_ = false;
        bool isRightMouseButtonPressed_ = false;
        glm::vec2 mousePosition_{0.0f};
        glm::vec2 lastMousePosition_{0.0f};
        glm::vec2 mouseDelta_{0.0f};
        float scrollDelta_ = 0.0f;

        static InputManager* instance_;
    };

} // namespace s3Dive


#endif //THREEDIVE_INPUTMANAGER_H
