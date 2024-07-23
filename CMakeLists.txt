cmake_minimum_required(VERSION 3.15)
project(ThreeDive C CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 REQUIRED)
find_package(glad REQUIRED)
find_package(imgui REQUIRED)
find_package(spdlog REQUIRED)
find_package(stb REQUIRED)
find_package(glm REQUIRED)
find_package(freetype REQUIRED)
find_package(assimp REQUIRED)
find_package(EnTT REQUIRED)

add_executable(ThreeDive
        source/main.cpp
        bindings/imgui_impl_glfw.cpp
        bindings/imgui_impl_glfw.h
        bindings/imgui_impl_opengl3.cpp
        bindings/imgui_impl_opengl3.h
        bindings/imgui_impl_opengl3_loader.h
        assets/simple-shader.vs.glsl
        assets/simple-shader.fs.glsl
        source/logging/debug_info.h
        source/platform/openGLRender/gl_shader.cpp
        source/platform/openGLRender/gl_shader.h
        source/platform/openGLRender/gl_shader_program.cpp
        source/platform/openGLRender/gl_shader_program.h
        source/platform/openGLRender/gl_vertex_array.cpp
        source/platform/openGLRender/gl_vertex_array.h
        source/platform/openGLRender/gl_vertex_buffer_layout.cpp
        source/platform/openGLRender/gl_vertex_buffer_layout.h
        source/platform/openGLRender/gl_vertex_buffer.cpp
        source/platform/openGLRender/gl_vertex_buffer.h
        source/platform/openGLRender/gl_index_buffer.cpp
        source/platform/openGLRender/gl_index_buffer.h
        source/platform/openGLRender/gl_texture.cpp
        source/platform/openGLRender/gl_texture.h
        source/platform/openGLRender/gl_uniform_buffer.cpp
        source/platform/openGLRender/gl_uniform_buffer.h
        source/platform/openGLRender/gl_renderer.cpp
        source/platform/openGLRender/gl_renderer.h
        source/camera/orthographic_camera.cpp
        source/camera/orthographic_camera.h
        source/camera/perspective_camera.cpp
        source/camera/perspective_camera.h
        source/camera/camera_controller.cpp
        source/camera/camera_controller.h
        source/events/event.h
        source/core/memory_and_binding.h
        source/events/key_codes.h
        source/core/app.cpp
        source/core/app.h
        source/core/window.cpp
        source/core/window.h
        source/renderer/RenderCommand.cpp
        source/renderer/RenderCommand.h
        source/events/event_type.h
        source/renderer/Renderer.cpp
        source/renderer/Renderer.h
        source/scene/scene.cpp
        source/scene/scene.h
        source/core/uuid.cpp
        source/core/uuid.h
        source/scene/components.h
        source/scene/SceneGridSystem.cpp
        source/scene/SceneGridSystem.h
        source/core/geo_generator.cpp
        source/core/geo_generator.h
        source/scene/entity.cpp
        source/scene/entity.h
)

add_custom_command(TARGET ThreeDive
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/assets/simple-shader.vs.glsl $<TARGET_FILE_DIR:ThreeDive>
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/assets/simple-shader.fs.glsl $<TARGET_FILE_DIR:ThreeDive>
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/assets/grid.vert $<TARGET_FILE_DIR:ThreeDive>
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/assets/grid.frag $<TARGET_FILE_DIR:ThreeDive>
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/assets/wall.jpg $<TARGET_FILE_DIR:ThreeDive>
        )

target_compile_definitions(ThreeDive PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW ENTT_INCLUDE_NATVIS)
target_link_libraries(ThreeDive imgui::imgui glfw glad::glad spdlog::spdlog stb::stb glm::glm freetype assimp::assimp EnTT::EnTT)

#add_subdirectory(tests)

