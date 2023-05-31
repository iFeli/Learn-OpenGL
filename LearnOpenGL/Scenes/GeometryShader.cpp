#include "GeometryShader.h"

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
	GeometryShader::GeometryShader() :
		pointsVAO(0),
		pointsVBO(0),
		matricesUBO(0),
		shader(
			Shader(
				"Resource Files/Shaders/GeometryShader.vert", 
				"Resource Files/Shaders/GeometryShader.geom", 
				"Resource Files/Shaders/GeometryShader.frag"
			)
		)
	{
		//
		// OpenGL
		//
		glClearColor(0.1f, 0.1f, 0.1f, 0.1f);

		glEnable(GL_DEPTH_TEST);

		//
		// Buffers.
		//

		// Points buffers.
		GLuint positionIndex = 0;

		glGenVertexArrays(1, &pointsVAO);
		glGenBuffers(1, &pointsVBO);

		glBindVertexArray(pointsVAO);

		glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(pointsVertices), pointsVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(positionIndex);
		glVertexAttribPointer(positionIndex, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

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

	GeometryShader::~GeometryShader()
	{
		glDeleteVertexArrays(1, &pointsVAO);
		glDeleteBuffers(1, &pointsVBO);
		glDeleteBuffers(1, &matricesUBO);
	}

	/*
	*
	* Public Methods
	*
	*/
	void GeometryShader::draw(const Camera& camera, const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection)
	{
		// OpenGL.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Uniform buffer.
		glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);

		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));

		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		// Draw the points.
		glBindVertexArray(pointsVAO);

		// Points.
		glm::mat4 translatedModel = glm::mat4(1.0f);
		translatedModel = glm::translate(translatedModel, glm::vec3(0.0f, 1.0f, 7.0f));

		shader.use();
		shader.setVector3("color", 0.0f, 1.0f, 0.0f);
		shader.setMatrix4("model", translatedModel);

		glDrawArrays(GL_POINTS, 0, 4);

		// Unbind the cubeVAO.
		glBindVertexArray(0);
	}

}