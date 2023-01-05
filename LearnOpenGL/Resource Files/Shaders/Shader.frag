#version 330 core

in vec3 vertexPosition;
in vec3 vertexColor;
in vec2 vertexTextureCoordinate;

out vec4 fragment_Color;

uniform sampler2D texture1Data;
uniform sampler2D texture2Data;
uniform float textureMix;

void main()
{
	fragment_Color = mix(texture(texture1Data, vertexTextureCoordinate), texture(texture2Data, vertexTextureCoordinate), textureMix);
}