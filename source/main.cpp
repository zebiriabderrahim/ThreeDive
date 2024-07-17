#include <spdlog/spdlog.h>

#include "core/app.h"



int main(int, char **) {
    auto window = std::make_shared<s3Dive::Window>();

    if (s3Dive::App app(window); app.initialize()) {
        app.run();
    }

    return 0;
}
