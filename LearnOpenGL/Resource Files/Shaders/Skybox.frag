#version 330 core

in vec3 textureCoordinates;

out vec4 frag_Color;

uniform samplerCube skyboxSampler;

void main()
{
	frag_Color = texture(skyboxSampler, textureCoordinates);
}