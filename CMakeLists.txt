cmake_minimum_required(VERSION 3.15)
project(ThreeDive C CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(imgui REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glad REQUIRED)

add_executable( ThreeDive
        main.cpp
        opengl_shader.cpp
        file_manager.cpp
        opengl_shader.h
        file_manager.h
        bindings/imgui_impl_glfw.cpp
        bindings/imgui_impl_glfw.h
        bindings/imgui_impl_opengl3.cpp
        bindings/imgui_impl_opengl3.h
        bindings/imgui_impl_opengl3_loader.h
        assets/simple-shader.vs
        assets/simple-shader.fs )

add_custom_command(TARGET ThreeDive
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/assets/simple-shader.vs $<TARGET_FILE_DIR:ThreeDive>
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/assets/simple-shader.fs $<TARGET_FILE_DIR:ThreeDive>
        )

target_compile_definitions(ThreeDive PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW)
target_link_libraries(ThreeDive imgui::imgui glfw glad::glad)
