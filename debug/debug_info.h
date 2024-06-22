//
// Created by ABDERRAHIM ZEBIRI on 2024-06-22.
//

#ifndef THREEDIVE_DEBUG_INFO_H
#define THREEDIVE_DEBUG_INFO_H

#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <string_view>

namespace s3Dive::debug {

#define GL_INFO(param) std::string_view(reinterpret_cast<const char*>(glGetString(param)))

    inline void glfw_error_callback(int error, const char *description)
    {
        spdlog::error("Glfw Error {}: {}", error, description);
    }

    inline void printGLInfo()
    {
        spdlog::info("OpenGL info:");
        spdlog::info("    Vendor:  {}",        GL_INFO(GL_VENDOR));
        spdlog::info("    Renderer:{}",        GL_INFO(GL_RENDERER));
        spdlog::info("    Version: {}",        GL_INFO(GL_VERSION));
        spdlog::info("    Shading version:{}", GL_INFO(GL_SHADING_LANGUAGE_VERSION));
    }

} // namespace s3Dive::debug

#endif //THREEDIVE_DEBUG_INFO_H
