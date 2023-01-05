#version 330 core

in vec3 vertexPosition;
in vec3 vertexColor;
in vec2 vertexTextureCoordinate;

out vec4 fragment_Color;

uniform bool colorize;
uniform sampler2D texture1Data;
uniform sampler2D texture2Data;
uniform float textureMix;

void main()
{
	vec4 color = mix(texture(texture1Data, vertexTextureCoordinate), texture(texture2Data, vertexTextureCoordinate), textureMix);

	if (colorize)
	{
		color = color * vec4(vertexColor, 1.0);
	}
	
	fragment_Color = color;
}