#version 330 core

layout (location = 0) in vec3 position;

out vec3 textureCoordinates;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	textureCoordinates = position;

	vec4 finalPosition = projection * view * vec4(position, 1.0);

	gl_Position = finalPosition.xyww;
}