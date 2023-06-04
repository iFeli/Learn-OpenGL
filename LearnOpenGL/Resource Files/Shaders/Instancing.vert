#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;

out vec3 vertexColor;

uniform vec2 offsets[100];

void main() 
{
	vertexColor = color;

	vec2 offset = offsets[gl_InstanceID];
	gl_Position = vec4(position + offset, 0.0, 1.0);
}