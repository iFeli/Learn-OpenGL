#version 330 core

struct Material 
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec2 textureCoordinates;
in vec3 position;
in vec3 normal;

out vec4 fragment_Color;

uniform Material material;
uniform Light light;

uniform vec3 viewPosition;

void main()
{
	// Normal and light direction.
	vec3 normalizedNormal = normalize(normal);
	vec3 lightDirection = normalize(light.position - position);

	// Diffuse.
	float diffuseImpact = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = light.diffuse * diffuseImpact * vec3(texture(material.diffuse, textureCoordinates));

	// Ambient.
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, textureCoordinates));

	// Specular.
	vec3 viewDirection = normalize(viewPosition - position);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float specularValue = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
	vec3 specular = light.specular * specularValue * vec3(texture(material.specular, textureCoordinates));

	// Final color.
	vec3 result = ambient + diffuse + specular;
	fragment_Color = vec4(result, 1.0);
}