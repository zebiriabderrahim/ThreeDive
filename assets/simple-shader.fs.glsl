#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;
uniform bool useTexture;
uniform sampler2D texture_diffuse1;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	// Ambient
	vec3 ambient = material.ambient;

	// Diffuse
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * material.diffuse;

	// Specular
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = material.specular * spec;

	vec3 result = ambient + diffuse + specular;

	if (useTexture)
	result *= texture(texture_diffuse1, TexCoords).rgb;

	FragColor = vec4(result, 1.0);
}