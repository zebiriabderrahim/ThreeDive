#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform float viewPos;
uniform vec3 lightColor;
uniform float ambientStrength;
uniform vec3 albedo;

void main()
{
	// Normalize the normal vector
	vec3 norm = normalize(Normal);

	// Calculate view direction
	vec3 viewDir = normalize(viewPos - FragPos);

	// Ambient lighting
	vec3 ambient = ambientStrength * lightColor;

	// Soft diffuse lighting
	float diff = max(dot(norm, vec3(0.0, 1.0, 0.0)), 0.0) * 0.5 + 0.5;
	vec3 diffuse = diff * lightColor;

	// Combine results
	vec3 result = (ambient + diffuse) * albedo;

	// Apply simple tone mapping
	result = result / (result + vec3(1.0));

	// Apply gamma correction
	result = pow(result, vec3(1.0/2.2));

	FragColor = vec4(result, 1.0);
}
