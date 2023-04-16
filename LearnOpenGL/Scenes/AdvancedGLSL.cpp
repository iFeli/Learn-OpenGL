#include "AdvancedGLSL.h"

#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <STB Image/stb_image.h>

#include <iostream>
#include <map>

namespace Pink
{

	/*
	*
	* Constructor & Destructor
	*
	*/
	AdvancedGLSL::AdvancedGLSL() :
		cubeVAO(0),
		cubeVBO(0),
		matricesUBO(0),
		shader(Shader("Resource Files/Shaders/UniformBufferObjects.vert", "Resource Files/Shaders/UniformBufferObjects.frag"))
	{
		//
		// OpenGL
		//
		glClearColor(0.1f, 0.1f, 0.1f, 0.1f);

		glEnable(GL_DEPTH_TEST);

		//
		// Buffers.
		//

		// Cube buffers.
		GLuint positionIndex = 0;

		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);

		glBindVertexArray(cubeVAO);

		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(positionIndex);
		glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// Uniform buffer.
		GLuint uniformBlockIndexRed = glGetUniformBlockIndex(shader.id, "Matrices");

		glUniformBlockBinding(shader.id, uniformBlockIndexRed, 0);

		glGenBuffers(1, &matricesUBO);

		glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
		glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, matricesUBO, 0, 2 * sizeof(glm::mat4));
	}

	AdvancedGLSL::~AdvancedGLSL()
	{
		glDeleteVertexArrays(1, &cubeVAO);
		glDeleteBuffers(1, &cubeVBO);
		glDeleteBuffers(1, &matricesUBO);
	}

	/*
	*
	* Public Methods
	*
	*/
	void AdvancedGLSL::draw(const Camera& camera, const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection)
	{
		// OpenGL.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Uniform buffer.
		glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);

		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));

		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		// Draw the cubes.
		glBindVertexArray(cubeVAO);
		
		// Red Cube.
		glm::mat4 translatedModel = glm::translate(model, glm::vec3(-1.0f, 2.0f, 5.0f));

		shader.use();
		shader.setVector3("color", 1.0f, 0.0f, 0.0f);
		shader.setMatrix4("model", translatedModel);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Green Cube.
		translatedModel = glm::translate(model, glm::vec3(1.0f, 2.0f, 5.0f));

		shader.use();
		shader.setVector3("color", 0.0f, 1.0f, 0.0f);
		shader.setMatrix4("model", translatedModel);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Blue cube.
		translatedModel = glm::translate(model, glm::vec3(1.0f, 0.0f, 5.0f));

		shader.use();
		shader.setVector3("color", 0.0f, 0.0f, 1.0f);
		shader.setMatrix4("model", translatedModel);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Yellow cube.
		translatedModel = glm::translate(model, glm::vec3(-1.0f, 0.0f, 5.0f));

		shader.use();
		shader.setVector3("color", 1.0f, 1.0f, 0.0f);
		shader.setMatrix4("model", translatedModel);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Unbind the cubeVAO.
		glBindVertexArray(0);
	}

}