#version 330 core
in vec4 vertexColor;
in float visibility;

uniform float detailVisibility1;
uniform float detailVisibility2;

out vec4 FragColor;

void main() {
    float alpha = vertexColor.a;
    if (visibility < 0.1) {
        alpha *= detailVisibility2;
    } else if (visibility < 0.6) {
        alpha *= detailVisibility1;
    }

    if (alpha < 0.01) {
        discard;
    }

    FragColor = vec4(vertexColor.rgb, alpha);
}
