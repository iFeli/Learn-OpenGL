#version 330 core
		
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 textureCoordinate;

//uniform float xOffset;

out vec3 vertexPosition;
out vec3 vertexColor;
out vec2 vertexTextureCoordinate;

void main()
{
	// Render the triangle upside down.
	//	gl_Position = vec4(-position, 1.0f);

	// Offset the triangle by a given uniform value.
	gl_Position = vec4(position, 1.0);

	vertexPosition = position;
	vertexColor = color;
	vertexTextureCoordinate = textureCoordinate;
};