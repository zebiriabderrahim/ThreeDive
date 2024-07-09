//
// Created by ABDERRAHIM ZEBIRI on 2024-07-08.
//

#ifndef THREEDIVE_WINDOW_H
#define THREEDIVE_WINDOW_H


#include <string>
#include <functional>
#include <GLFW/glfw3.h>
#include "memory_and_binding.h"
#include "event.h"
#include "../openGLRender/gl_context.h"

namespace s3Dive {

class Window {
public:
    using EventCallbackFn = std::function<void(const EventVariant&)>;
    struct WindowProps {
        std::string title;
        uint32_t width;
        uint32_t height;

        explicit WindowProps(std::string title = "3Dive", uint32_t width = 1600, uint32_t height = 900)
            : title(std::move(title)), width(width), height(height) {}
    };

    explicit Window(const WindowProps& props = WindowProps{});
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;

    void onUpdate();
    void processEvents();

    [[nodiscard]] uint32_t getWidth() const { return data_.width; }
    [[nodiscard]] uint32_t getHeight() const { return data_.height; }
    [[nodiscard]] bool isVSync() const { return data_.vSync; }
    [[nodiscard]] bool isInitialized() const { return window_ != nullptr; }

    void setVSync(bool enabled);
    [[nodiscard]] GLFWwindow* getNativeWindow() const { return window_; }

    void addEventListener(EventType type, const EventCallbackFn& callback) {
        eventDispatcher_.addEventListener(type, callback);
    }

private:
    void init(const WindowProps& props);
    void setGlfwCallbacks();
    void shutdown();

    static void glfwErrorCallback(int error, const char* description);

    GLFWwindow* window_{nullptr};
    UniquePtr<GLContext> context_;
    EventDispatcher eventDispatcher_;
    EventQueue eventQueue_;

    struct WindowData {
        std::string title;
        uint32_t width{0};
        uint32_t height{0};
        bool vSync{false};
    };

    WindowData data_;
};

} // namespace s3Dive

#endif //THREEDIVE_WINDOW_H
