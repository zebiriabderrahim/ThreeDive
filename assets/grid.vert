#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in float aVisibility;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float detailVisibility;

out vec4 vertexColor;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    // Apply detail visibility only to detail lines (aVisibility == 0.0)
    float finalAlpha = aColor.a;
    if (aVisibility < 0.5) {
        finalAlpha *= detailVisibility;
    }

    vertexColor = vec4(aColor.rgb, finalAlpha);
}