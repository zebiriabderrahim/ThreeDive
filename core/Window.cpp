#include "Window.h"
#include <spdlog/spdlog.h>

namespace s3Dive {

    static uint8_t s_GLFWWindowCount = 0;

    Window::Window(const WindowProps& props) {
        init(props);
    }

    Window::~Window() {
        shutdown();
    }

    void Window::init(const WindowProps& props) {
        data_.title = props.title;
        data_.width = props.width;
        data_.height = props.height;

        spdlog::info("Creating window {0} ({1}, {2})", props.title, props.width, props.height);

        if (s_GLFWWindowCount == 0) {
            if (!glfwInit()) {
                spdlog::error("Failed to initialize GLFW");
                return;
            }

            // Decide GL+GLSL versions
#if __APPLE__
            // GL 3.2 + GLSL 150
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
            // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
            glfwSetErrorCallback(glfwErrorCallback);
        }

        window_ = glfwCreateWindow(static_cast<int>(props.width), static_cast<int>(props.height), data_.title.c_str(), nullptr, nullptr);
        if (!window_) {
            throw std::runtime_error("Failed to create GLFW window");
        }
        ++s_GLFWWindowCount;

        context_ = MakeUnique<GLContext>(window_);
        context_->init();

        glfwSetWindowUserPointer(window_, this);
        setVSync(true);

        setGlfwCallbacks();
    }

    void Window::setGlfwCallbacks() {
        glfwSetWindowUserPointer(window_, this);

        glfwSetMouseButtonCallback(window_, [](GLFWwindow* window, int button, int action, int mods) {
            auto* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
            double xpos;
            double ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            if (action == GLFW_PRESS) {
                win->eventQueue_.enqueueEvent(MouseButtonPressedEvent{static_cast<MouseCode>(button),
                                                                      static_cast<float>(xpos), static_cast<float>(ypos)});
            } else if (action == GLFW_RELEASE) {
                win->eventQueue_.enqueueEvent(MouseButtonReleasedEvent{static_cast<MouseCode>(button),
                                                                       static_cast<float>(xpos), static_cast<float>(ypos)});
            }
        });

        glfwSetCursorPosCallback(window_, [](GLFWwindow* window, double xpos, double ypos) {
            auto* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
            win->eventQueue_.enqueueEvent(MouseMovedEvent{static_cast<float>(xpos),
                                                          static_cast<float>(ypos)});
        });

        glfwSetScrollCallback(window_, [](GLFWwindow* window, double xoffset, double yoffset) {
            auto* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
            win->eventQueue_.enqueueEvent(MouseScrolledEvent{static_cast<float>(xoffset),
                                                             static_cast<float>(yoffset)});
        });

        glfwSetFramebufferSizeCallback(window_, [](GLFWwindow* window, int width, int height) {
            auto* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
            win->eventQueue_.enqueueEvent(WindowResizeEvent{static_cast<unsigned int>(width),
                                                            static_cast<unsigned int>(height)});
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
        context_->swapBuffers();
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
        data_.vSync = enabled;
    }

    void Window::glfwErrorCallback(int error, const char* description) {
        spdlog::error("GLFW Error ({0}): {1}", error, description);
    }

} // namespace s3Dive
