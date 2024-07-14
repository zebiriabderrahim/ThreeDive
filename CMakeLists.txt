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
        source/renderer/ortho_camera.cpp
        source/renderer/ortho_camera.h
        source/renderer/ICamera.h
        source/renderer/perspective_camera.cpp
        source/renderer/perspective_camera.h
        source/renderer/CameraController.cpp
        source/renderer/CameraController.h
        source/scene/scene_node.cpp
        source/scene/scene_node.h source/core/event.h
        source/core/memory_and_binding.h
        source/core/key_codes.h
        source/core/app.cpp
        source/core/app.h
        source/core/geo_util.h
        source/core/Window.cpp
        source/core/Window.h
        source/scene/SceneGrid.cpp
        source/scene/SceneGrid.h
#        scene/ViewCube.cpp
#        scene/ViewCube.h
        source/renderer/RenderCommand.cpp source/renderer/RenderCommand.h)

add_custom_command(TARGET ThreeDive
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/assets/simple-shader.vs.glsl $<TARGET_FILE_DIR:ThreeDive>
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/assets/simple-shader.fs.glsl $<TARGET_FILE_DIR:ThreeDive>
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/assets/grid.vert $<TARGET_FILE_DIR:ThreeDive>
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/assets/grid.frag $<TARGET_FILE_DIR:ThreeDive>
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/assets/wall.jpg $<TARGET_FILE_DIR:ThreeDive>
        )

target_compile_definitions(ThreeDive PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW)
target_link_libraries(ThreeDive imgui::imgui glfw glad::glad spdlog::spdlog stb::stb glm::glm freetype)

#add_subdirectory(tests)

