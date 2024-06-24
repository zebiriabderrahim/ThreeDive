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

    if (std::remove(testFilePath.c_str()) != 0) {
        std::cerr << "Error deleting file: " << testFilePath << std::endl;
    }
}

TEST(ShaderTest, LoadShaderFileEmpty) {
    const std::string testFilePath = "empty_shader.glsl";
    std::ofstream outFile(testFilePath);
    outFile.close();

    std::string result = Shader::loadShaderFile(testFilePath);
    EXPECT_EQ(result, "");

    if (std::remove(testFilePath.c_str()) != 0) {
        std::cerr << "Error deleting file: " << testFilePath << std::endl;
    }
}

TEST(ShaderTest, LoadShaderFileFailure) {

    auto result = Shader::loadShaderFile("non_existing_file.glsl");

    EXPECT_EQ(result, "");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
