#include "FaceCulling.h"

#include <STB Image/stb_image.h>

#include <iostream>

namespace Pink
{

	/*
	*
	* Constructor & Destructor
	*
	*/
	FaceCulling::FaceCulling() :
		cubeTexture(0),
		cubeVAO(0),
		cubeVBO(0),
		textureShader(Shader("Resource Files/Shaders/Texture.vert", "Resource Files/Shaders/Texture.frag"))
	{
		//
		// OpenGL.
		//
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		//
		// Textures.
		//
		int width = 0;
		int height = 0;
		int numberOfChannels = 0;
		unsigned char* imageData = nullptr;

		glGenTextures(1, &cubeTexture);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		imageData = stbi_load("Resource Files/Textures/Marble.jpg", &width, &height, &numberOfChannels, 0);

		if (imageData)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load the cube's texture." << std::endl;
		}

		stbi_image_free(imageData);

		width = 0;
		height = 0;
		numberOfChannels = 0;
		imageData = nullptr;

		glBindTexture(GL_TEXTURE_2D, 0);

		//
		// Buffers.
		//
		const GLuint positionIndex = 0;
		const GLuint textureIndex = 1;

		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);

		glBindVertexArray(cubeVAO);

		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(positionIndex);
		glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
		glEnableVertexAttribArray(textureIndex);
		glVertexAttribPointer(textureIndex, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)(3 * sizeof(float)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}

	FaceCulling::~FaceCulling()
	{
		glDeleteTextures(1, &cubeTexture);

		glDeleteVertexArrays(1, &cubeVAO);

		glDeleteBuffers(1, &cubeVBO);
	}

	/*
	*
	* Public Methods
	*
	*/
	void FaceCulling::draw(const Camera& camera, const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection)
	{
		glClear(GL_COLOR_BUFFER_BIT);

		textureShader.use();
		textureShader.setInteger("textureSampler", 0);

		textureShader.setMatrix4("model", model);
		textureShader.setMatrix4("view", view);
		textureShader.setMatrix4("projection", projection);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);
	}

}