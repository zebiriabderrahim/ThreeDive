#include <gtest/gtest.h>
#include <fstream>

#include "../openGLRender/shader.h"


using namespace s3Dive;

TEST(ShaderTest, LoadShaderFileSuccess) {
    const std::string testFilePath = "test_shader.glsl";
    const std::string testShaderCode = "#version 330 core\n"
                                       "\n"
                                       "layout (location = 0) in vec3 position;\n"
                                       "layout (location = 1) in vec3 color;\n"
                                       "\n"
                                       "out vec3 vertexColor;\n"
                                       "\n"
                                       "uniform float rotation;\n"
                                       "uniform vec2 translation;\n"
                                       "\n"
                                       "void main()\n"
                                       "{\n"
                                       "\tvec2 rotated_pos;\n"
                                       "\trotated_pos.x = translation.x + position.x*cos(rotation) - position.y*sin(rotation);\n"
                                       "\trotated_pos.y = translation.y + position.x*sin(rotation) + position.y*cos(rotation);\n"
                                       "    gl_Position = vec4(rotated_pos.x, rotated_pos.y, position.z, 1.0);\n"
                                       "\tvertexColor = color;\n"
                                       "}";

    std::ofstream outFile(testFilePath);
    outFile << testShaderCode;
    outFile.close();

    std::string result = Shader::loadShaderFile(testFilePath);

    EXPECT_EQ(result, testShaderCode);

    std::remove(testFilePath.c_str());
}

TEST(ShaderTest, LoadShaderFileEmpty) {
    // Create an empty file
    const std::string testFilePath = "empty_shader.glsl";
    std::ofstream outFile(testFilePath);
    outFile.close();

    // Attempt to load the empty file and expect an empty string as the result
    std::string result = Shader::loadShaderFile(testFilePath);
    EXPECT_EQ(result, "");

    // Remove the empty file
    std::remove(testFilePath.c_str());
}

TEST(ShaderTest, LoadShaderFileFailure) {
    // Attempt to load a non-existing file and expect an empty string as the result
    std::string result;
    try {
        result = Shader::loadShaderFile("non_existing_file.glsl");
    } catch (const std::ifstream::failure& e) {
        FAIL() << "Unexpected exception thrown: " << e.what();
    }

    // Ensure that the result is an empty string
    EXPECT_EQ(result, "");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
