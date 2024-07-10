# Add gtest dependency
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add an executable for the tests
add_executable(test_shader test_shader.cpp
        ../openGLRender/gl_shader.cpp
        ../openGLRender/gl_shader_program.cpp
        ../openGLRender/gl_shader_program.h)

# Link with gtest and the main project libraries
target_link_libraries(test_shader ${CONAN_LIBS} GTest::GTest GTest::Main glad::glad spdlog::spdlog)

# Enable testing
enable_testing()

# Add the test
add_test(NAME test_shader COMMAND test_shader)
