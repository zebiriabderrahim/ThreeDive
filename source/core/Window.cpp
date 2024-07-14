#include <spdlog/spdlog.h>

#include "../logging/debug_info.h"
#include "Window.h"


namespace s3Dive {

    Window::Window(const WindowProps &props) {
        init(props);
    }

    Window::~Window() {
        shutdown();
    }

    void Window::init(const WindowProps &props) {
        title_ = props.title;
        width_ = props.width;
        height_ = props.height;

        spdlog::info("Creating window {} ({}, {})", props.title, props.width, props.height);

        if (s_GLFWWindowCount == 0) {
            if (!glfwInit()) {
                spdlog::error("Failed to initialize GLFW");
                return;
            }

#if __APPLE__
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif
            glfwSetErrorCallback(glfwErrorCallback);
        }

        window_ = glfwCreateWindow(props.width, props.height, title_.c_str(), nullptr, nullptr);
        if (!window_) {
            spdlog::error("Failed to create GLFW window");
            return;
        }
        ++s_GLFWWindowCount;

        glfwMakeContextCurrent(window_);

        // Initialize OpenGL loader
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            spdlog::error("Failed to initialize OpenGL context");
            return;
        }

        s3Dive::debug::printGLInfo();

        glfwSetWindowUserPointer(window_, this);
        setVSync(true);

        setGlfwCallbacks();
    }

    void Window::setGlfwCallbacks() {
        glfwSetWindowUserPointer(window_, this);

        glfwSetMouseButtonCallback(window_, [](GLFWwindow *window, int button, int action, [[maybe_unused]] int mods) {
            auto *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
            double xPos;
            double yPos;
            glfwGetCursorPos(window, &xPos, &yPos);
            if (action == GLFW_PRESS) {
                win->eventQueue_.enqueueEvent(MouseButtonPressedEvent{static_cast<MouseCode>(button),
                                                                      static_cast<float>(xPos),
                                                                      static_cast<float>(yPos)});
            } else if (action == GLFW_RELEASE) {
                win->eventQueue_.enqueueEvent(MouseButtonReleasedEvent{static_cast<MouseCode>(button),
                                                                       static_cast<float>(xPos),
                                                                       static_cast<float>(yPos)});
            }
        });

        glfwSetCursorPosCallback(window_, [](GLFWwindow *window, double xPos, double yPos) {
            auto *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
            win->eventQueue_.enqueueEvent(MouseMovedEvent{static_cast<float>(xPos),static_cast<float>(yPos)});
        });

        glfwSetScrollCallback(window_, [](GLFWwindow *window, double xOffset, double yOffset) {
            auto *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
            win->eventQueue_.enqueueEvent(MouseScrolledEvent{static_cast<float>(xOffset),static_cast<float>(yOffset)});
        });

        glfwSetFramebufferSizeCallback(window_, [](GLFWwindow *window, int width, int height) {
            auto *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
            win->eventQueue_.enqueueEvent(WindowResizeEvent{width, height});
            glViewport(0, 0, width, height);
        });

        glfwSetWindowCloseCallback(window_, [](GLFWwindow* window) {
            auto* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
            win->eventQueue_.enqueueEvent(WindowCloseEvent{});
        });
    }

    void Window::shutdown() {
        if (window_) {
            glfwDestroyWindow(window_);
            --s_GLFWWindowCount;

            if (s_GLFWWindowCount == 0) {
                glfwTerminate();
            }
        }
    }

    void Window::onUpdate() {
        if (!isInitialized()) return;

        glfwPollEvents();
        processEvents();
        glfwSwapBuffers(window_);
    }

    void Window::processEvents() {
        eventQueue_.processEvents(eventDispatcher_);
    }

    void Window::setVSync(bool enabled) {
        if (enabled) {
            glfwSwapInterval(1);
        } else {
            glfwSwapInterval(0);
        }
        vSync_ = enabled;
    }
    bool Window::shouldClose() const {
        return glfwWindowShouldClose(window_);
    }


    void Window::glfwErrorCallback(int error, const char *description) {
        spdlog::error("GLFW Error ({0}): {1}", error, description);
    }

    void Window::addEventListener(EventType type, const Window::EventCallbackFn &callback) {
            eventDispatcher_.addEventListener(type, callback);
    }

} // namespace s3Dive
