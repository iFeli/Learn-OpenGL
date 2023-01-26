#include "Blending.h"

#include <GLM/gtc/matrix_transform.hpp>
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
	Blending::Blending() :
		cubeTexture(0),
		floorTexture(0),
		grassTexture(0),
		windowTexture(0),
		cubeVAO(0),
		floorVAO(0),
		planeVAO(0),
		cubeVBO(0),
		floorVBO(0),
		planeVBO(0),
		alphaShader(Shader("Resource Files/Shaders/Alpha.vert", "Resource Files/Shaders/Alpha.frag")),
		textureShader(Shader("Resource Files/Shaders/Texture.vert", "Resource Files/Shaders/Texture.frag"))
	{
		//
		// OpenGL
		//
		glClearColor(0.1f, 0.1f, 0.1f, 0.1f);

		glEnable(GL_LINE_SMOOTH);
		glLineWidth(3.0f);

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Variables
		planePositions.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
		planePositions.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
		planePositions.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
		planePositions.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
		planePositions.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

		//
		// Textures.
		//
		int width = 0;
		int height = 0;
		int numberOfChannels = 0;
		unsigned char* imageData = nullptr;

		stbi_set_flip_vertically_on_load(true);

		// Cube texture.
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

		// Floor texture.
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
			std::cout << "Failed to load the floor's texture." << std::endl;
		}

		stbi_image_free(imageData);

		width = 0;
		height = 0;
		numberOfChannels = 0;
		imageData = nullptr;

		glBindTexture(GL_TEXTURE_2D, 0);

		// Grass texture.
		glGenTextures(1, &grassTexture);
		glBindTexture(GL_TEXTURE_2D, grassTexture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		
		stbi_set_flip_vertically_on_load(false);

		imageData = stbi_load("Resource Files/Textures/Grass.png", &width, &height, &numberOfChannels, 0);

		if (imageData)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load the grass texture." << std::endl;
		}

		stbi_image_free(imageData);

		width = 0;
		height = 0;
		numberOfChannels = 0;
		imageData = nullptr;

		glBindTexture(GL_TEXTURE_2D, 0);

		// Window texture.
		glGenTextures(1, &windowTexture);
		glBindTexture(GL_TEXTURE_2D, windowTexture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		imageData = stbi_load("Resource Files/Textures/Window.png", &width, &height, &numberOfChannels, 0);

		if (imageData)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load the window's texture." << std::endl;
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

		// Cube buffers.
		GLuint positionIndex = 0;
		GLuint textureIndex = 1;

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

		// Floor buffers.
		glGenVertexArrays(1, &floorVAO);
		glGenBuffers(1, &floorVBO);

		glBindVertexArray(floorVAO);

		glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(positionIndex);
		glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
		glEnableVertexAttribArray(textureIndex);
		glVertexAttribPointer(textureIndex, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)(3 * sizeof(float)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// Plane buffers.
		glGenVertexArrays(1, &planeVAO);
		glGenBuffers(1, &planeVBO);

		glBindVertexArray(planeVAO);

		glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(grassVertices), grassVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(positionIndex);
		glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
		glEnableVertexAttribArray(textureIndex);
		glVertexAttribPointer(textureIndex, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)(3 * sizeof(float)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	Blending::~Blending()
	{
		glDeleteTextures(1, &cubeTexture);
		glDeleteTextures(1, &floorTexture);
		glDeleteTextures(1, &grassTexture);
		glDeleteTextures(1, &windowTexture);

		glDeleteVertexArrays(1, &cubeVAO);
		glDeleteVertexArrays(1, &floorVAO);
		glDeleteVertexArrays(1, &planeVAO);

		glDeleteBuffers(1, &cubeVBO);
		glDeleteBuffers(1, &floorVBO);
		glDeleteBuffers(1, &planeVBO);
	}

	/*
	*
	* Public Methods
	*
	*/
	void Blending::draw(const Camera& camera, const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection)
	{
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw the floor.
		textureShader.use();
		textureShader.setInteger("textureSampler", 0);

		textureShader.setMatrix4("model", model);
		textureShader.setMatrix4("view", view);
		textureShader.setMatrix4("projection", projection);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floorTexture);

		glBindVertexArray(floorVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);

		// Draw the cube.
		textureShader.use();
		textureShader.setInteger("textureSampler", 1);

		textureShader.setMatrix4("model", model);
		textureShader.setMatrix4("view", view);
		textureShader.setMatrix4("projection", projection);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glBindVertexArray(0);

		// Draw the grass.
		/*alphaShader.use();
		alphaShader.setInteger("textureSampler", 2);

		alphaShader.setMatrix4("view", view);
		alphaShader.setMatrix4("projection", projection);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, grassTexture);

		glBindVertexArray(planeVAO);

		for (unsigned int i = 0; i < planePositions.size(); i++)
		{
			glm::mat4 planeModel = glm::mat4(1.0f);
			planeModel = glm::translate(planeModel, planePositions[i]);
			alphaShader.setMatrix4("model", planeModel);

			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		glBindVertexArray(0);*/
		
		// Draw the windows.
		textureShader.use();
		textureShader.setInteger("textureSampler", 3);

		textureShader.setMatrix4("view", view);
		textureShader.setMatrix4("projection", projection);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, windowTexture);

		glBindVertexArray(planeVAO);

		std::map<float, glm::vec3> sortedPlanePositions;

		for (unsigned int i = 0; i < planePositions.size(); i++)
		{
			float distance = glm::length(camera.position - planePositions[i]);
			sortedPlanePositions[distance] = planePositions[i];
		}

		for (std::map<float, glm::vec3>::reverse_iterator iterator = sortedPlanePositions.rbegin();
			iterator != sortedPlanePositions.rend(); 
			iterator++)
		{
			glm::mat4 planeModel = glm::mat4(1.0f);
			planeModel = glm::translate(planeModel, iterator->second);
			alphaShader.setMatrix4("model", planeModel);

			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		glBindVertexArray(0);
	}

}