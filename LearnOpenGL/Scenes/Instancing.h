#ifndef INSTANCING_H
#define INSTANCING_H

#include "Scene.h"
#include "Shader.h"

namespace Pink 
{

	class Instancing: public Scene
	{
	public:
		Instancing();
		~Instancing();

		void draw(const Camera& camera, const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection);

	private:
		GLuint quadVAO;
		GLuint quadVBO;

		Shader shader;

		glm::vec2 translations[100];

		float quadVertices[30] = {
			// Positions     // Colors
			-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
			 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
			-0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

			-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
			 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
			 0.05f,  0.05f,  0.0f, 1.0f, 1.0f
		};
	};

}

#endif