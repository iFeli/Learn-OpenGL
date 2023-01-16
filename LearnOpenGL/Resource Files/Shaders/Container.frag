#version 330 core

/*
*
* Structures
*
*/
struct Material 
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirectionalLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position; 

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight
{
	vec3 position; 
	vec3 direction;
	float cutoff;
	float outerCutoff;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

/*
*
* Defines
*
*/
#define NUMBER_OF_POINT_LIGHTS 4

/*
*
* In
*
*/
in vec3 position;
in vec3 normal;
in vec2 textureCoordinates;

/*
*
* Out
*
*/
out vec4 fragment_Color;

/*
*
* Uniform
*
*/
uniform vec3 viewPosition;

uniform Material material;
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[NUMBER_OF_POINT_LIGHTS];
uniform SpotLight spotLight;

/*
*
* Function Declarations.
*
*/
vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection);
vec3 calculatePointlLight(PointLight light, vec3 normal, vec3 position, vec3 viewDirection);
vec3 calculateSpotlLight(SpotLight light, vec3 normal, vec3 position, vec3 viewDirection);

/*
*
* Main 
*
*/

void main()
{
	// View direction.
	vec3 viewDirection = normalize(viewPosition - position);

	// Normal.
	vec3 normalizedNormal = normalize(normal);

	// Light is set up in 3 phases:
	// Directional, point, and spot lights.
	// For each phase, use calculate function to determine corresponding color per lamp.
	// In the main() function, take all of the calculated colors and add them up for the
	// fragment's final color.

	// Phase 1: Directional lighting.
	vec3 result = calculateDirectionalLight(directionalLight, normal, viewDirection);

	// Phase 2: Point lights.
	for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++)
	{
		result += calculatePointlLight(pointLights[i], normal, position, viewDirection);
	}

	// Phase 3: Spot light.
	result += calculateSpotlLight(spotLight, normal, position, viewDirection);
	
	// Final, resulting fragment color.
	fragment_Color = vec4(result, 1.0);
}

/*
*
* Function Definitions.
*
*/
vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection)
{
	vec3 lightDirection = normalize(-light.direction);
	vec3 reflectionDirection = reflect(-lightDirection, normal);

	float diffuseFactor = max(dot(normal, lightDirection), 0.0);
	float specularFactor =  pow(max(dot(viewDirection, reflectionDirection), 0.0), material.shininess);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, textureCoordinates));
	vec3 diffuse = light.diffuse * diffuseFactor * vec3(texture(material.diffuse, textureCoordinates));
	vec3 specular = light.specular * specularFactor * vec3(texture(material.specular, textureCoordinates));

	return (ambient + diffuse + specular);
}

vec3 calculatePointlLight(PointLight light, vec3 normal, vec3 position, vec3 viewDirection)
{
	vec3 lightDirection = normalize(light.position - position);
	vec3 reflectionDirection = reflect(-lightDirection, normal);

	float diffuseFactor = max(dot(normal, lightDirection), 0.0);
	float specularFactor =  pow(max(dot(viewDirection, reflectionDirection), 0.0), material.shininess);

	float distance = length(light.position - position);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, textureCoordinates));
	vec3 diffuse = light.diffuse * diffuseFactor * vec3(texture(material.diffuse, textureCoordinates));
	vec3 specular = light.specular * specularFactor * vec3(texture(material.specular, textureCoordinates));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 calculateSpotlLight(SpotLight light, vec3 normal, vec3 position, vec3 viewDirection)
{
	vec3 lightDirection = normalize(light.position - position);
	vec3 reflectionDirection = reflect(-lightDirection, normal);

	float diffuseFactor = max(dot(normal, lightDirection), 0.0);
	float specularFactor =  pow(max(dot(viewDirection, reflectionDirection), 0.0), material.shininess);

	float distance = length(light.position - position);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	float theta = dot(lightDirection, normalize(-light.direction));
	float epsilon = light.cutoff - light.outerCutoff;
	float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, textureCoordinates));
	vec3 diffuse = light.diffuse * diffuseFactor * vec3(texture(material.diffuse, textureCoordinates));
	vec3 specular = light.specular * specularFactor * vec3(texture(material.specular, textureCoordinates));

	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return (ambient + diffuse + specular);

}