//
// Created by ABDERRAHIM ZEBIRI on 2024-07-07.
//

#include "InputManager.h"

namespace s3Dive {

        InputManager* InputManager::instance_ = nullptr;

        InputManager::InputManager(GLFWwindow* window) : window_(window) {
            instance_ = this;
        }

        InputManager::~InputManager() {
            if (instance_ == this) {
                instance_ = nullptr;
            }
        }

        void InputManager::update() {
            double xpos, ypos;
            glfwGetCursorPos(window_, &xpos, &ypos);
            glm::vec2 currentMousePos(static_cast<float>(xpos), static_cast<float>(ypos));
            mouseDelta_ = currentMousePos - lastMousePosition_;
            lastMousePosition_ = currentMousePos;
            mousePosition_ = currentMousePos;
        }

        bool InputManager::isKeyPressed(int key) const {
            return glfwGetKey(window_, key) == GLFW_PRESS;
        }

        void InputManager::setMousePosition(double xpos, double ypos) {
            mousePosition_ = glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos));
        }

        void InputManager::setMouseButton(int button, bool pressed) {
            switch (button) {
                case GLFW_MOUSE_BUTTON_LEFT:
                    isLeftMouseButtonPressed_ = pressed;
                    break;
                case GLFW_MOUSE_BUTTON_MIDDLE:
                    isMiddleMouseButtonPressed_ = pressed;
                    break;
                case GLFW_MOUSE_BUTTON_RIGHT:
                    isRightMouseButtonPressed_ = pressed;
                    break;
            }
        }

        void InputManager::addScrollDelta(float delta) {
            scrollDelta_ += delta;
        }

        void InputManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
            setMouseButton(button, action == GLFW_PRESS);
        }

        void InputManager::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
            setMousePosition(xpos, ypos);
        }

        void InputManager::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
            addScrollDelta(static_cast<float>(yoffset));
        }

        void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
            // Implement if needed
        }


} // namespace s3Dive
