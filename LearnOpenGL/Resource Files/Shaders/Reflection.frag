#version 330 core

in vec3 position;
in vec3 normal;

out vec4 frag_Color;

uniform vec3 cameraPosition;
uniform samplerCube skyboxSampler;

void main()
{
	vec3 viewDirection = normalize(position - cameraPosition);
	vec3 reflection = reflect(viewDirection, normalize(normal));

	frag_Color = vec4(texture(skyboxSampler, reflection).rgb, 1.0);
}