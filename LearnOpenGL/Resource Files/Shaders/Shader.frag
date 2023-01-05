#version 330 core

in vec3 vertexPosition;
in vec3 vertexColor;
in vec2 vertexTextureCoordinate;

out vec4 fragment_Color;

uniform sampler2D textureData;

void main()
{
	fragment_Color = texture(textureData, vertexTextureCoordinate) * vec4(vertexColor, 1.0);
//	fragment_Color = vec4(vertexColor, 1.0);
}