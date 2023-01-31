#version 330 core

in vec3 position;
in vec3 normal;

out vec4 frag_Color;

uniform vec3 cameraPosition;
uniform samplerCube skyboxSampler;

void main()
{
	float ratio = 1.00 / 1.52;

	vec3 viewDirection = normalize(position - cameraPosition);
	vec3 refraction = refract(viewDirection, normalize(normal), ratio);

	frag_Color = vec4(texture(skyboxSampler, refraction).rgb, 1.0);
}