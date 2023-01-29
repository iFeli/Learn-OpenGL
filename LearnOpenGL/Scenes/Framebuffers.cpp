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
		fboTexture(0),
		cubeVAO(0),
		cubeVBO(0),
		quadVAO(0),
		quadVBO(0),
		fbo(0),
		rbo(0),
		colorShader(Shader("Resource Files/Shaders/Color.vert", "Resource Files/Shaders/Color.frag")),
		quadTextureShader(Shader("Resource Files/Shaders/QuadTexture.vert", "Resource Files/Shaders/QuadTexture.frag")),
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
		glDeleteFramebuffers(1, &fbo);

		glDeleteTextures(1, &cubeTexture);
		glDeleteTextures(1, &fboTexture);

		glDeleteVertexArrays(1, &cubeVAO);

		glDeleteBuffers(1, &cubeVBO);
	}

	/*
	* 
	* Public Methods
	* 
	*/
	void Framebuffers::draw(const Camera& camera, const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection)
	{
		// Render to default framebuffer.
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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

		glBindTexture(GL_TEXTURE_2D, 0);

		// Render to custom framebuffer.
		const GLsizei framebufferWidth = WIDTH / 2;
		const GLsizei framebufferHeight = HEIGHT / 2;

		glViewport(0, 0, framebufferWidth, framebufferHeight);

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);

		glClearColor(0.8f, 0.1f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		colorShader.use();
		colorShader.setVector3("color", 0.2f, 1.0f, 0.6f);

		colorShader.setMatrix4("model", model);
		colorShader.setMatrix4("view", view);
		colorShader.setMatrix4("projection", projection);

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, WIDTH, HEIGHT);

		// Render the plane with the custom framebuffer's color attachment texture as its texture.
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);

		quadTextureShader.use();
		quadTextureShader.setInteger("textureSampler", 1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, fboTexture);

		glBindVertexArray(quadVAO);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

}