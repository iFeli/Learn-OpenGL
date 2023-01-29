#ifndef FACE_CULLING_H
#define FACE_CULLING_H

#include "Scene.h"
#include "Shader.h"

namespace Pink
{

	class FaceCulling : public Scene
	{
	public:
		FaceCulling();
		~FaceCulling();

		void draw(const Camera& camera, const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection);

	private:
		GLuint cubeTexture;

		GLuint cubeVAO;
		GLuint cubeVBO;

		Shader textureShader;

        float cubeVertices[180] = {
            // Back face
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // Top-right
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // Bottom-right         
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // Top-right
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // Top-left
            // Front face
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // Bottom-left
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // Bottom-right
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // Top-right
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // Top-right
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // Top-left
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // Bottom-left
            // Left face
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // Top-right
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // Top-left
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // Bottom-left
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // Bottom-left
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // Bottom-right
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // Top-right
            // Right face
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // Top-left
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // Bottom-right
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // Top-right         
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // Bottom-right
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // Top-left
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // Bottom-left     
             // Bottom face
             -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // Top-right
              0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
              0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // Bottom-left
              0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // Bottom-left
             -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // Bottom-right
             -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // Top-right
             // Top face
             -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // Top-left
              0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // Bottom-right
              0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // Top-right     
              0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // Bottom-right
             -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // Top-left
             -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // Bottom-left        
        };
	};

}

#endif