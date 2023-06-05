#ifndef MSAA_H
#define MSAA_H

#include "Scene.h"
#include "Shader.h"

namespace Pink {

	class MSAA: public Scene
	{
	public:
		MSAA();
		~MSAA();

		void draw(const Camera& camera, const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection);

	private:
		GLuint cubeVAO;
		GLuint cubeVBO;

		Shader shader;

        const float cubeVertices[108] = {
            // Back face
            -0.5f, -0.5f, -0.5f,  // Bottom-left
             0.5f,  0.5f, -0.5f,  // Top-right
             0.5f, -0.5f, -0.5f,  // Bottom-right         
             0.5f,  0.5f, -0.5f,  // Top-right
            -0.5f, -0.5f, -0.5f,  // Bottom-left
            -0.5f,  0.5f, -0.5f,  // Top-left
            // Front face
            -0.5f, -0.5f,  0.5f,  // Bottom-left
             0.5f, -0.5f,  0.5f,  // Bottom-right
             0.5f,  0.5f,  0.5f,  // Top-right
             0.5f,  0.5f,  0.5f,  // Top-right
            -0.5f,  0.5f,  0.5f,  // Top-left
            -0.5f, -0.5f,  0.5f,  // Bottom-left
            // Left face
            -0.5f,  0.5f,  0.5f,  // Top-right
            -0.5f,  0.5f, -0.5f,  // Top-left
            -0.5f, -0.5f, -0.5f,  // Bottom-left
            -0.5f, -0.5f, -0.5f,  // Bottom-left
            -0.5f, -0.5f,  0.5f,  // Bottom-right
            -0.5f,  0.5f,  0.5f,  // Top-right
            // Right face
             0.5f,  0.5f,  0.5f,  // Top-left
             0.5f, -0.5f, -0.5f,  // Bottom-right
             0.5f,  0.5f, -0.5f,  // Top-right         
             0.5f, -0.5f, -0.5f,  // Bottom-right
             0.5f,  0.5f,  0.5f,  // Top-left
             0.5f, -0.5f,  0.5f,  // Bottom-left     
             // Bottom face
             -0.5f, -0.5f, -0.5f, // Top-right
              0.5f, -0.5f, -0.5f, // top-left
              0.5f, -0.5f,  0.5f, // Bottom-left
              0.5f, -0.5f,  0.5f, // Bottom-left
             -0.5f, -0.5f,  0.5f, // Bottom-right
             -0.5f, -0.5f, -0.5f, // Top-right
             // Top face
             -0.5f,  0.5f, -0.5f, // Top-left
              0.5f,  0.5f,  0.5f, // Bottom-right
              0.5f,  0.5f, -0.5f, // Top-right     
              0.5f,  0.5f,  0.5f, // Bottom-right
             -0.5f,  0.5f, -0.5f, // Top-left
             -0.5f,  0.5f,  0.5f, // Bottom-left        
        };
	};

}

#endif