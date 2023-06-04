#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 offset;

out vec3 vertexColor;

uniform vec2 offsets[100];

void main() 
{
	vertexColor = color;
	
	vec2 adjustedPosition = position * (gl_InstanceID / 100.0);
	gl_Position = vec4(adjustedPosition + offset, 0.0, 1.0);
}