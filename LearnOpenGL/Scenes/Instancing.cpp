#include "Instancing.h"

#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

namespace Pink
{

	/*
	*
	* Constructor & Destructor
	*
	*/
	Instancing::Instancing() :
		quadVAO(0),
		quadVBO(0),
		translationsVBO(0),
		shader(Shader("Resource Files/Shaders/Instancing.vert", "Resource Files/Shaders/Instancing.frag"))
	{
		//
		// Translations
		//
		int index = 0;
		float offset = 0.1f;

		const int translationsCount = 100;

		glm::vec2 translations[translationsCount];

		for (int y = -10; y < 10; y += 2)
		{
			for (int x = -10; x < 10; x += 2)
			{
				glm::vec2 translation = glm::vec2((float)x / 10.0f + offset, (float)y / 10.0f + offset);
				translations[index++] = translation;
			}
		}

		//
		// OpenGL
		//
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		glEnable(GL_DEPTH_TEST);

		//
		// Buffers
		//
		GLuint positionIndex = 0;
		GLuint colorIndex = 1;
		GLuint offsetIndex = 2;

		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glGenBuffers(1, &translationsVBO);

		glBindVertexArray(quadVAO);

		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(positionIndex);
		glVertexAttribPointer(positionIndex, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);

		glEnableVertexAttribArray(colorIndex);
		glVertexAttribPointer(colorIndex, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)(2 * sizeof(float)));

		glBindBuffer(GL_ARRAY_BUFFER, translationsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(offsetIndex);
		glVertexAttribPointer(offsetIndex, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
		glVertexAttribDivisor(2, 1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}

	Instancing::~Instancing()
	{
		glDeleteVertexArrays(1, &quadVAO);
		glDeleteBuffers(1, &quadVBO);
	}

	/*
	*
	* Public Methods
	*
	*/
	void Instancing::draw(const Camera& camera, const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection)
	{
		// OpenGL
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw Instanced Quads
		shader.use();

		glBindVertexArray(quadVAO);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
		glBindVertexArray(0);
	}

}