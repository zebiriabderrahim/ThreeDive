#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 vertexColor;
uniform vec3 color;
uniform sampler2D tex;

void main()
{
	FragColor = texture(tex, TexCoord) * vec4(color, 1.0);
}
