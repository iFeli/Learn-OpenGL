#version 330 core

in vec2 textureCoordinates;

out vec4 frag_Color;

uniform sampler2D textureSampler;

const float offset = 1.0 / 300.0;

void main()
{
	vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // Top-left
        vec2( 0.0f,    offset), // Top-center
        vec2( offset,  offset), // Top-right
        vec2(-offset,  0.0f),   // Center-left
        vec2( 0.0f,    0.0f),   // Center-center
        vec2( offset,  0.0f),   // Center-right
        vec2(-offset, -offset), // Bottom-left
        vec2( 0.0f,   -offset), // Bottom-center
        vec2( offset, -offset)  // Bottom-right    
    );

    // Sharpen kernel.
    /*
    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
    */

    // Blur kernel.
    /*
    float kernel[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16  
    );
    */

    // Edge detection kernel.
    float kernel[9] = float[](
        1,  1, 1,
        1, -8, 1,
        1,  1, 1
    );

    vec3 samples[9];
    vec3 color = vec3(0.0);

    for(int i = 0; i < 9; i++)
    {
        samples[i] = vec3(texture(textureSampler, textureCoordinates.st + offsets[i]));
        
        color += samples[i] * kernel[i];
    }

    frag_Color = vec4(color, 1.0);
}