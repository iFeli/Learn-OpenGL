#include "Framebuffers.h"

#include <STB Image/stb_image.h>

#include <iostream>

#include "Variables.h"

namespace Pink
{

	/*
	* 
	* Constructor & Destructor
	* 
	*/
	Framebuffers::Framebuffers() :
		cubeTexture(0),
		floorTexture(0),
		fboTexture(0),
		cubeVAO(0),
		cubeVBO(0),
		floorVAO(0),
		floorVBO(0),
		quadVAO(0),
		quadVBO(0),
		fbo(0),
		rbo(0),
		colorShader(Shader("Resource Files/Shaders/Color.vert", "Resource Files/Shaders/Color.frag")),
		postProcessShader(Shader("Resource Files/Shaders/TextureQuad.vert", "Resource Files/Shaders/TextureKernel.frag")),
		textureShader(Shader("Resource Files/Shaders/Texture.vert", "Resource Files/Shaders/Texture.frag"))
	{
		//
		// OpenGL.
		//
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);

		//
		// Textures.
		//
		createTexture(cubeTexture, "Resource Files/Textures/Container.jpg");
		createTexture(floorTexture, "Resource Files/Textures/Metal.png");

		//
		// Buffers.
		//
		GLuint positionIndex = 0;
		GLuint textureIndex = 1;

		// Cube buffers.
		glGenVertexArrays(1, &cubeVAO);
		glBindVertexArray(cubeVAO);

		glGenBuffers(1, &cubeVBO);
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
		glBindVertexArray(floorVAO);

		glGenBuffers(1, &floorVBO);
		glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(positionIndex);
		glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
		glEnableVertexAttribArray(textureIndex);
		glVertexAttribPointer(textureIndex, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		// Quad buffers.
		glGenVertexArrays(1, &quadVAO);
		glBindVertexArray(quadVAO);

		glGenBuffers(1, &quadVBO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(positionIndex);
		glVertexAttribPointer(positionIndex, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
		glEnableVertexAttribArray(textureIndex);
		glVertexAttribPointer(textureIndex, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)(2 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		//
		// Framebuffer.
		//
		const GLsizei framebufferWidth = WIDTH / 2;
		const GLsizei framebufferHeight = HEIGHT / 2;

		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		// Color texture/attachment.
		glGenTextures(1, &fboTexture);
		glBindTexture(GL_TEXTURE_2D, fboTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, framebufferWidth, framebufferHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0); 
		glBindTexture(GL_TEXTURE_2D, 0);
		
		// Depth and stencil renderbuffer.
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, framebufferWidth, framebufferHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); 
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "Failed to properly create the off-screen framebuffer." << std::endl;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	Framebuffers::~Framebuffers()
	{
		glDeleteTextures(1, &cubeTexture);
		glDeleteTextures(1, &floorTexture);
		glDeleteTextures(1, &fboTexture);

		glDeleteVertexArrays(1, &cubeVAO);
		glDeleteBuffers(1, &cubeVBO);
		glDeleteVertexArrays(1, &floorVAO);
		glDeleteBuffers(1, &floorVBO);
		glDeleteVertexArrays(1, &quadVAO);
		glDeleteBuffers(1, &quadVBO);

		glDeleteFramebuffers(1, &fbo);
		glDeleteRenderbuffers(1, &rbo);
	}

	/*
	* 
	* Public Methods
	* 
	*/
	void Framebuffers::draw(const Camera& camera, const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection)
	{
		// Render to default framebuffer.
		drawScene(model, view, projection);

		// Render to custom framebuffer.
		const GLsizei framebufferWidth = WIDTH / 2;
		const GLsizei framebufferHeight = HEIGHT / 2;

		glViewport(0, 0, framebufferWidth, framebufferHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		drawScene(model, view, projection);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, WIDTH, HEIGHT);

		// Render the plane with the custom framebuffer's color attachment texture as its texture.
		drawScenePostProcessed();
	}

	/*
	*
	* Private Methods
	*
	*/
	void Framebuffers::createTexture(GLuint& texture, std::string_view filename)
	{
		int width = 0;
		int height = 0;
		int numberOfChannels = 0;
		unsigned char* imageData = nullptr;

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		imageData = stbi_load(filename.data(), &width, &height, &numberOfChannels, 0);

		if (imageData)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
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

	void Framebuffers::drawScene(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
	{
		//
		// Set up OpenGL.
		//
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);

		glClearColor(0.8f, 0.1f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		//
		// Draw the cubes.
		//
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		glBindVertexArray(cubeVAO);

		textureShader.use();
		textureShader.setInteger("textureSampler", 0);
		textureShader.setMatrix4("view", view);
		textureShader.setMatrix4("projection", projection);

		// First cube.
		textureShader.setMatrix4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Second cube.
		glm::mat4 secondModel = glm::mat4(1.0f);
		secondModel = glm::translate(secondModel, glm::vec3(2.0f, 0.0f, 2.0f));
		textureShader.setMatrix4("model", secondModel);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		// 
		// Draw the floor.
		//
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		glBindVertexArray(floorVAO);

		textureShader.use();
		textureShader.setInteger("textureSampler", 1);
		textureShader.setMatrix4("model", model);
		textureShader.setMatrix4("view", view);
		textureShader.setMatrix4("projection", projection);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Framebuffers::drawScenePostProcessed()
	{
		//
		// Set up OpenGL.
		//
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, fboTexture);
		glBindVertexArray(quadVAO);

		postProcessShader.use();
		postProcessShader.setInteger("textureSampler", 1);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}