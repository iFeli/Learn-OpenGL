#version 330 core

in vec2 textureCoordinates;

out vec4 frag_Color;

uniform sampler2D textureSampler;

void main()
{
	frag_Color = texture(textureSampler, textureCoordinates);
}