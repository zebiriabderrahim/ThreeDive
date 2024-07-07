#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include "openGLRender/gl_context.h"
#include "openGLRender/gl_renderer.h"
#include "core/App.h"



int main(int, char **) {
    s3Dive::GLRenderer renderer;
    renderer.init();
    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(800, 600, "s3Dive Test Application", nullptr, nullptr);
    if (window == nullptr) {
        spdlog::error("Failed to create GLFW window");
        return 1;
    }


    s3Dive::GLContext context(window);
    context.init();
    s3Dive::TestApplication app(window);
    if (app.initialize()) {
        app.run();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
