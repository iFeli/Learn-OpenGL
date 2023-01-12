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
	vec3 direction;
	float cutoff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
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
	// Light direction.
	vec3 lightDirection = normalize(light.position - position);

	// Check if lighting is inside the spotlight cone.
	float theta = dot(lightDirection, normalize(-light.direction));

	if (theta > light.cutoff)
	{
		// Normal.
		vec3 normalizedNormal = normalize(normal);

		// Diffuse.
		float diffuseImpact = max(dot(normal, lightDirection), 0.0);
		vec3 diffuse = light.diffuse * diffuseImpact * texture(material.diffuse, textureCoordinates).rgb;

		// Ambient.
		vec3 ambient = light.ambient * texture(material.diffuse, textureCoordinates).rgb;

		// Specular.
		vec3 viewDirection = normalize(viewPosition - position);
		vec3 reflectDirection = reflect(-lightDirection, normal);
		float specularValue = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
		vec3 specular = light.specular * specularValue * texture(material.specular, textureCoordinates).rgb;

		// Attenuation.
		float distance = length(light.position - position);
		float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

		diffuse *= attenuation;

		// Remove ambient attenuation for ambient color. Otherwise, at large distances, thelight would be 
		// darker inside than outside the spotlight due to the ambient term in the else branch.
		// ambient *= attenuation;

		specular *= attenuation;

		// Final color.
		vec3 result = ambient + diffuse + specular;
	fragment_Color = vec4(result, 1.0);
//		fragment_Color = vec4(0.8, 0.1, 0.5, 1.0);
}
	else
	{
		// Otherwise, use the ambient light so the scene isn't completely dark outside the spotlight.
		fragment_Color = vec4(light.ambient * texture(material.diffuse, textureCoordinates).rgb, 1.0);
	}
}