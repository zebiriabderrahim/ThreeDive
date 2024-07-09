cmake_minimum_required(VERSION 3.15)
project(ThreeDive C CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 REQUIRED)
find_package(glad REQUIRED)
find_package(imgui REQUIRED)
find_package(spdlog REQUIRED)
find_package(stb REQUIRED)
find_package(glm REQUIRED)

add_executable(ThreeDive
        main.cpp
        bindings/imgui_impl_glfw.cpp
        bindings/imgui_impl_glfw.h
        bindings/imgui_impl_opengl3.cpp
        bindings/imgui_impl_opengl3.h
        bindings/imgui_impl_opengl3_loader.h
        assets/simple-shader.vs.glsl
        assets/simple-shader.fs.glsl
        logging/debug_info.h
        openGLRender/shader.cpp
        openGLRender/shader.h
        openGLRender/gl_shader_program.cpp
        openGLRender/gl_shader_program.h
        openGLRender/gl_vertex_array.cpp
        openGLRender/gl_vertex_array.h
        openGLRender/gl_vertex_buffer_layout.cpp
        openGLRender/gl_vertex_buffer_layout.h
        openGLRender/gl_vertex_buffer.cpp
        openGLRender/gl_vertex_buffer.h
        openGLRender/gl_index_buffer.cpp
        openGLRender/gl_index_buffer.h
        openGLRender/gl_texture.cpp
        openGLRender/gl_texture.h
        openGLRender/gl_uniform_buffer.cpp
        openGLRender/gl_uniform_buffer.h
        openGLRender/gl_context.cpp
        openGLRender/gl_context.h
        openGLRender/gl_renderer.cpp
        openGLRender/gl_renderer.h
        renderer/ortho_camera.cpp
        renderer/ortho_camera.h
        renderer/ICamera.h
        renderer/perspective_camera.cpp
        renderer/perspective_camera.h
        renderer/CameraController.cpp
        renderer/CameraController.h
        scene/scene_node.cpp
        scene/scene_node.h core/event.h
        core/memory_and_binding.h
        core/key_codes.h
        core/app.cpp
        core/app.h core/InputManager.cpp core/InputManager.h core/geo_util.h core/Window.cpp core/Window.h)

add_custom_command(TARGET ThreeDive
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/assets/simple-shader.vs.glsl $<TARGET_FILE_DIR:ThreeDive>
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/assets/simple-shader.fs.glsl $<TARGET_FILE_DIR:ThreeDive>
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/assets/grid.vert $<TARGET_FILE_DIR:ThreeDive>
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/assets/grid.frag $<TARGET_FILE_DIR:ThreeDive>
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/assets/wall.jpg $<TARGET_FILE_DIR:ThreeDive>
        )

target_compile_definitions(ThreeDive PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW)
target_link_libraries(ThreeDive imgui::imgui glfw glad::glad spdlog::spdlog stb::stb glm::glm)

#add_subdirectory(tests)

