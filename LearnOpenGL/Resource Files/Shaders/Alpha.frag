#version 330 core

in vec2 textureCoordinates;

out vec4 frag_Color;

uniform sampler2D textureSampler;

void main()
{
	vec4 color = texture(textureSampler, textureCoordinates);

	if (color.a < 0.1)
	{
		discard;
	}

	frag_Color = color;
}