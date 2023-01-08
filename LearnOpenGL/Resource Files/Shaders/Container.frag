#version 330 core

in vec3 fragmentPosition;
in vec3 vertexNormal;

out vec4 fragment_Color;

uniform vec3 color;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;

void main()
{
	// Normal.
	vec3 normal = normalize(vertexNormal);
	vec3 lightDirection = normalize(lightPosition - fragmentPosition);

	// Diffuse.
	float diffuseImpact = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = diffuseImpact * lightColor;

	// Ambient.
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// Specular.
	float specularStrength = 0.5;
	vec3 viewDirection = normalize(viewPosition - fragmentPosition);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float specularValue = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
	vec3 specular = specularStrength * specularValue * lightColor;

	// Final color.
	vec3 result = (ambient + diffuse + specular) * color;
	fragment_Color = vec4(result, 1.0);
}