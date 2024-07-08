#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in float aVisibility;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float detailVisibility1;
uniform float detailVisibility2;

out vec4 vertexColor;
out float visibility;

void main() {
    vec4 viewPos = view * model * vec4(aPos, 1.0);
    gl_Position = projection * viewPos;

    visibility = aVisibility;
    vertexColor = aColor;
}
