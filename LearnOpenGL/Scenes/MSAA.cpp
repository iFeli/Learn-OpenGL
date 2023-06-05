#include "MSAA.h"

#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

namespace Pink
{

	/*
	*
	* Constructor & Destructor
	*
	*/
	MSAA::MSAA() :
		cubeVAO(0),
		cubeVBO(0),
		shader(Shader("Resource Files/Shaders/Color.vert", "Resource Files/Shaders/Color.frag"))
	{
		// 
		// OpenGL
		// 
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);

		//
		// Buffers
		//
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
	}

	MSAA::~MSAA()
	{
		glDeleteVertexArrays(1, &cubeVAO);
		glDeleteBuffers(1, &cubeVBO);
	}

	/*
	*
	* Public Methods
	*
	*/
	void MSAA::draw(const Camera& camera, const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection)
	{
		//
		// OpenGL
		//
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//
		// Draw Cube
		//
		shader.use();
		shader.setMatrix4("model", model);
		shader.setMatrix4("view", view);
		shader.setMatrix4("projection", projection);
		shader.setVector3("color", glm::vec3(1.0f, 0.0f, 0.3f));


		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

}