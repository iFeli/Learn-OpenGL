#include "StencilTest.h"

#include <GLM/gtc/type_ptr.hpp>
#include <STB Image/stb_image.h>

#include <iostream>

namespace Pink
{

	/*
	*
	* Constructor & Destructor
	*
	*/
	StencilTest::StencilTest() :
		cubeTexture(0),
		cubeVAO(0),
		cubeVBO(0),
		floorTexture(0),
		floorVAO(0),
		floorVBO(0),
		colorShader(Shader("Resource Files/Shaders/Color.vert", "Resource Files/Shaders/Color.frag")),
		textureShader(Shader("Resource Files/Shaders/Texture.vert", "Resource Files/Shaders/Texture.frag"))
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		GLuint positionIndex = 0;
		GLuint textureIndex = 1;

		int width = 0;
		int height = 0;
		int numberOfChannels = 0;
		unsigned char* imageData = nullptr;

		stbi_set_flip_vertically_on_load(true);

		// Cube.
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
			std::cout << "Failed to load the cube texture." << std::endl;
		}

		stbi_image_free(imageData);

		imageData = nullptr;

		glBindTexture(GL_TEXTURE_2D, 0);

		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);

		glBindVertexArray(cubeVAO);

		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(positionIndex);
		glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)0);
		glEnableVertexAttribArray(textureIndex);
		glVertexAttribPointer(textureIndex, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)(3 * sizeof(float)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// Floor.
		glGenTextures(1, &floorTexture);
		glBindTexture(GL_TEXTURE_2D, floorTexture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		imageData = stbi_load("Resource Files/Textures/Metal.png", &width, &height, &numberOfChannels, 0);

		if (imageData)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load the floor texture." << std::endl;
		}

		stbi_image_free(imageData);

		imageData = nullptr;

		glBindTexture(GL_TEXTURE_2D, 0);

		glGenVertexArrays(1, &floorVAO);
		glGenBuffers(1, &floorVBO);

		glBindVertexArray(floorVAO);

		glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), &floorVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(positionIndex);
		glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)0);
		glEnableVertexAttribArray(textureIndex);
		glVertexAttribPointer(textureIndex, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)(3 * sizeof(float)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	StencilTest::~StencilTest()
	{
		glDeleteTextures(1, &cubeTexture);
		glDeleteBuffers(1, &cubeVBO);
		glDeleteVertexArrays(1, &cubeVAO);

		glDeleteTextures(1, &floorTexture);
		glDeleteBuffers(1, &floorVBO);
		glDeleteVertexArrays(1, &floorVAO);
	}
	
	/*
	*
	* Public Methods
	*
	*/
	void StencilTest::draw(const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glStencilMask(0x00); // Don't accidentally update the stencil buffer while drawing.

		// 
		// Draw the floor.
		//
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floorTexture);

		glBindVertexArray(floorVAO);

		textureShader.use();
		textureShader.setInteger("textureSampler", 0);

		textureShader.setMatrix4("model", glm::value_ptr(model));
		textureShader.setMatrix4("view", glm::value_ptr(view));
		textureShader.setMatrix4("projection", glm::value_ptr(projection));

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);

		//
		// Draw the cube.
		//
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		glBindVertexArray(cubeVAO);

		textureShader.use();
		textureShader.setInteger("textureSampler", 1);

		textureShader.setMatrix4("model", glm::value_ptr(model));
		textureShader.setMatrix4("view", glm::value_ptr(view));
		textureShader.setMatrix4("projection", glm::value_ptr(projection));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		//
		// Draw the border of the cube.
		//
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);

		glDisable(GL_DEPTH_TEST);

		glBindVertexArray(cubeVAO);

		float scale = 1.05f;
		glm::mat4 scaledModel = glm::mat4(1.0f);
		scaledModel = glm::scale(scaledModel, glm::vec3(scale, scale, scale));

		colorShader.use();
		colorShader.setVector3("color", glm::vec3(0.5f, 0.1f, 0.2f));

		colorShader.setMatrix4("model", glm::value_ptr(scaledModel));
		colorShader.setMatrix4("view", glm::value_ptr(view));
		colorShader.setMatrix4("projection", glm::value_ptr(projection));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);

		glEnable(GL_DEPTH_TEST);
	}
}