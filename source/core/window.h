//
// Created by ABDERRAHIM ZEBIRI on 2024-07-08.
//

#ifndef THREEDIVE_WINDOW_H
#define THREEDIVE_WINDOW_H

#include <string>
#include <functional>
#include <utility>
#include <GLFW/glfw3.h>
#include "memory_and_binding.h"
#include "event.h"

namespace s3Dive {

    class Window {
    public:
        using EventCallbackFn = std::function<void(const EventVariant&)>;
        struct WindowProps {
            std::string title;
            int width;
            int height;

            explicit WindowProps(std::string  title = "3Dive Engine",
                        int width = 960,
                        int height = 720)
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

        [[nodiscard]] bool isInitialized() const { return window_ != nullptr; }

        void setVSync(bool enabled);
        [[nodiscard]] GLFWwindow* getNativeWindow() const { return window_; }
        [[nodiscard]] int getWidth() const { return width_; }
        [[nodiscard]] int getHeight() const { return height_; }
        [[nodiscard]] bool isVSync() const { return vSync_; }
        [[nodiscard]] bool shouldClose() const;

        void addEventListener(EventType type, const EventCallbackFn& callback);
    private:
        void init(const WindowProps& props);
        void setGlfwCallbacks();
        void shutdown();
        static void glfwErrorCallback(int error, const char* description);

        GLFWwindow* window_{nullptr};
        EventDispatcher eventDispatcher_;
        EventQueue eventQueue_;

        std::string title_;
        int width_{0};
        int height_{0};
        bool vSync_{false};

        inline static int s_GLFWWindowCount = 0;
    };

} // namespace s3Dive

#endif //THREEDIVE_WINDOW_H
