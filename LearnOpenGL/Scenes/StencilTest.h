#ifndef STENCIL_TEST_H
#define STENCIL_TEST_H

#include <GLAD/glad.h>

#include "GLM/gtc/type_ptr.hpp"

#include "Scene.h"
#include "Shader.h"

namespace Pink
{

	class StencilTest : public Scene
	{
	private:
		GLuint cubeVAO;
		GLuint cubeVBO;
		GLuint floorVAO;
		GLuint floorVBO;

		Shader basicShader;

		float floorVertices[18] = {
			 5.0f, -0.5f,  5.0f,
			-5.0f, -0.5f,  5.0f,
			-5.0f, -0.5f, -5.0f,
					   
			 5.0f, -0.5f,  5.0f,
			-5.0f, -0.5f, -5.0f,
			 5.0f, -0.5f, -5.0f
		};

		float cubeVertices[108] = {
			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,

			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,

			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,

			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f, -0.5f,
							   
			-0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f
		};

	public: 
		StencilTest() :
			cubeVAO(0),
			cubeVBO(0),
			floorVAO(0),
			floorVBO(0),
			basicShader(Shader("Resource Files/Shaders/Basic.vert", "Resource Files/Shaders/Basic.frag"))
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);

			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

			GLuint positionIndex = 0;

			// Cube.
			glGenVertexArrays(1, &cubeVAO);
			glGenBuffers(1, &cubeVBO);

			glBindVertexArray(cubeVAO);

			glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);

			glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
			glEnableVertexAttribArray(positionIndex);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			// Floor.
			glGenVertexArrays(1, &floorVAO);
			glGenBuffers(1, &floorVBO);

			glBindVertexArray(floorVAO);

			glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), &floorVertices, GL_STATIC_DRAW);

			glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
			glEnableVertexAttribArray(positionIndex);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		~StencilTest()
		{
			glDeleteBuffers(1, &cubeVBO);
			glDeleteVertexArrays(1, &cubeVAO);

			glDeleteBuffers(1, &floorVBO);
			glDeleteVertexArrays(1, &floorVAO);
		}

		void draw(const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			glStencilMask(0x00); // Don't accidentally update the stencil buffer while drawing.

			// 
			// Draw the floor.
			//

			glBindVertexArray(floorVAO);

			basicShader.use();
			basicShader.setVector3("color", glm::vec3(0.4f, 0.4f, 0.5f));

			basicShader.setMatrix4("model", glm::value_ptr(model));
			basicShader.setMatrix4("view", glm::value_ptr(view));
			basicShader.setMatrix4("projection", glm::value_ptr(projection));

			glDrawArrays(GL_TRIANGLES, 0, 6);

			glBindVertexArray(0);

			//
			// Draw the cube.
			//
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);

			glBindVertexArray(cubeVAO);

			basicShader.use();
			basicShader.setVector3("color", glm::vec3(0.5f, 0.1f, 0.2f));

			basicShader.setMatrix4("model", glm::value_ptr(model));
			basicShader.setMatrix4("view", glm::value_ptr(view));
			basicShader.setMatrix4("projection", glm::value_ptr(projection));

			glDrawArrays(GL_TRIANGLES, 0, 36);

			glBindVertexArray(0);

			//
			// Draw the border of the cube.
			//
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);

			glDisable(GL_DEPTH_TEST);

			glBindVertexArray(cubeVAO);

			float scale = 1.1f;
			glm::mat4 scaledModel = glm::mat4(1.0f);
			scaledModel = glm::scale(scaledModel, glm::vec3(scale, scale, scale));

			basicShader.use();
			basicShader.setVector3("color", glm::vec3(0.2f, 0.1f, 0.5f));

			basicShader.setMatrix4("model", glm::value_ptr(scaledModel));
			basicShader.setMatrix4("view", glm::value_ptr(view));
			basicShader.setMatrix4("projection", glm::value_ptr(projection));

			glDrawArrays(GL_TRIANGLES, 0, 36);

			glBindVertexArray(0);

			glStencilMask(0xFF);
			glStencilFunc(GL_ALWAYS, 1, 0xFF);

			glEnable(GL_DEPTH_TEST);
		}
	};

}

#endif