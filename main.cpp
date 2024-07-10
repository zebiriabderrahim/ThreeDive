#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include "openGLRender/gl_context.h"
#include "openGLRender/gl_renderer.h"
#include "core/app.h"



int main(int, char **) {
    auto window = std::make_shared<s3Dive::Window>();

    s3Dive::GLRenderer renderer;
    renderer.init();
    if (s3Dive::TestApplication app(window); app.initialize()) {
        app.run();
    }

    return 0;
}
