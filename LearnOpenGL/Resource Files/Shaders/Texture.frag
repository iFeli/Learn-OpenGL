#version 330 core

in VS_OUT
{
	vec2 textureCoordinates;
} fs_in;

out vec4 frag_Color;

uniform sampler2D textureSampler;

void main()
{
	if (gl_FrontFacing)
	{
		frag_Color = texture(textureSampler, fs_in.textureCoordinates);
	}
	else
	{
		frag_Color = vec4(1.0, 0.0, 0.3, 1.0);
	}
}