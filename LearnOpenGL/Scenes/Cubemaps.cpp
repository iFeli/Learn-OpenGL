#include "Cubemaps.h"

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
	Cubemaps::Cubemaps() :
		cubemap(0),
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
		containerShader(Shader("Resource Files/Shaders/Reflection.vert", "Resource Files/Shaders/Refraction.frag")),
		skyboxShader(Shader("Resource Files/Shaders/Skybox.vert", "Resource Files/Shaders/Skybox.frag")),
		textureShader(Shader("Resource Files/Shaders/Texture.vert", "Resource Files/Shaders/Texture.frag"))
	{
		//
		// OpenGL.
		//
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_LINE_SMOOTH);
		glLineWidth(3.0f);

		//
		// Variables.
		//
		planePositions.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
		planePositions.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
		planePositions.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
		planePositions.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
		planePositions.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

		//
		// Textures.
		//
		
		// Cubemap.
		std::vector<std::string> faces{
			"Right.jpg",
			"Left.jpg",
			"Top.jpg",
			"Bottom.jpg",
			"Front.jpg",
			"Back.jpg"
		};

		loadCubemap(cubemap, faces);

		// Cube.
		createTexture(cubeTexture, "Resource Files/Textures/Marble.jpg");

		// Floor.
		createTexture(floorTexture, "Resource Files/Textures/Metal.png", false, true);

		// Grass.
		createTexture(grassTexture, "Resource Files/Textures/Grass.png", true);

		// Window.
		createTexture(windowTexture, "Resource Files/Textures/Window.png", true);

		//
		// Buffers.
		//
		GLuint positionIndex = 0;
		GLuint normalIndex = 1;
		GLuint textureIndex = 1;

		// Cubemap.
		glGenVertexArrays(1, &cubemapVAO);
		glGenBuffers(1, &cubemapVBO);

		glBindVertexArray(cubemapVAO);

		glBindBuffer(GL_ARRAY_BUFFER, cubemapVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(positionIndex);
		glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// Cube.
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);

		glBindVertexArray(cubeVAO);

		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(positionIndex);
		glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
		glEnableVertexAttribArray(normalIndex);
		glVertexAttribPointer(normalIndex, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(3 * sizeof(float)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// Floor.
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

		// Plane.
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

	Cubemaps::~Cubemaps()
	{
		glDeleteTextures(1, &cubemap);
		glDeleteTextures(1, &cubeTexture);
		glDeleteTextures(1, &floorTexture);
		glDeleteTextures(1, &grassTexture);
		glDeleteTextures(1, &windowTexture);

		glDeleteVertexArrays(1, &cubemapVAO);
		glDeleteVertexArrays(1, &cubeVAO);
		glDeleteVertexArrays(1, &floorVAO);
		glDeleteVertexArrays(1, &planeVAO);

		glDeleteBuffers(1, &cubemapVBO);
		glDeleteBuffers(1, &cubeVBO);
		glDeleteBuffers(1, &floorVBO);
		glDeleteBuffers(1, &planeVBO);
	}

	/*
	*
	* Public Methods
	*
	*/
	void Cubemaps::draw(const Camera& camera, const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		drawScene(camera, model, view, projection);

		drawSkybox(camera, projection);
	}

	/*
	*
	* Private Methods
	*
	*/
	void Cubemaps::createTexture(GLuint& texture, std::string_view filename, bool hasAlpha, bool wrapRepeat)
	{
		int width = 0;
		int height = 0;
		int numberOfChannels = 0;
		unsigned char* imageData = nullptr;

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		GLint parameter = wrapRepeat ? GL_REPEAT : GL_CLAMP_TO_BORDER;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, parameter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, parameter);

		imageData = stbi_load(filename.data(), &width, &height, &numberOfChannels, 0);

		if (imageData)
		{
			GLint format = hasAlpha ? GL_RGBA : GL_RGB;

			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
			glGenerateMipmap(GL_TEXTURE_2D);			
		}
		else
		{
			std::cout << "Failed to load the texture." << std::endl;
		}

		stbi_image_free(imageData);

		width = 0;
		height = 0;
		numberOfChannels = 0;
		imageData = nullptr;

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Cubemaps::drawScene(const Camera& camera, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
	{
		// Draw the floor.
		glEnable(GL_CULL_FACE);

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
		glBindTexture(GL_TEXTURE_2D, 0);

		glDisable(GL_CULL_FACE);

		// Draw the cube.
		containerShader.use();
		containerShader.setInteger("skyboxSampler", 0);
		containerShader.setVector3("cameraPosition", camera.position);
		containerShader.setMatrix4("model", model);
		containerShader.setMatrix4("view", view);
		containerShader.setMatrix4("projection", projection);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		// Draw the grass.
		alphaShader.use();
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

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Cubemaps::drawSkybox(const Camera& camera, const glm::mat4& projection)
	{
		glDepthFunc(GL_LEQUAL);

		glm::mat4 updatedView = glm::mat4(glm::mat3(camera.getViewMatrix()));

		skyboxShader.use();
		skyboxShader.setInteger("skyboxSampler", 0);
		skyboxShader.setMatrix4("view", updatedView);
		skyboxShader.setMatrix4("projection", projection);

		glBindVertexArray(cubemapVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glBindVertexArray(0);

		glDepthFunc(GL_LESS);
	}

	void Cubemaps::loadCubemap(GLuint& cubemap, const std::vector<std::string> faces)
	{
		glGenTextures(1, &cubemap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

		int width = 0;
		int height = 0;
		int numberOfChannels = 0;

		stbi_set_flip_vertically_on_load(false);

		for (unsigned int i = 0; i < faces.size(); i++)
		{
			const std::string path = "Resource Files/Textures/Skybox/" + faces[i];

			unsigned char* imageData = stbi_load(path.c_str(), &width, &height, &numberOfChannels, 0);

			if (imageData)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
			}
			else
			{
				std::cout << "Failed to load an image that makes up the skybox." << std::endl;
			}

			stbi_image_free(imageData);

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		}

	}

}