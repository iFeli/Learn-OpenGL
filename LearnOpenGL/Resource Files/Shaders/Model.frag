#version 330 core

out vec4 fragmentColor;

in vec2 textureCoordinates;

uniform sampler2D texture_diffuse1;

void main()
{    
    fragmentColor = texture(texture_diffuse1, textureCoordinates);
}