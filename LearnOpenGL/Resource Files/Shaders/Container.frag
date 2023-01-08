#version 330 core

out vec4 fragment_Color;

uniform vec3 color;
uniform vec3 lightColor;

void main()
{
	fragment_Color = vec4(color * lightColor, 1.0f);
}