#version 330 core

in vec2 textureCoordinates;

out vec4 frag_Color;

uniform sampler2D textureSampler;

void main()
{
	frag_Color = texture(textureSampler, textureCoordinates);
	float average = (0.2126 * frag_Color.r) + (0.7152 * frag_Color.g) + (0.0722 * frag_Color.b);
	frag_Color = vec4(average, average, average, 1.0);
}