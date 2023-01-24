#include "Renderer.h"

#include "Camera.h"
#include "Model.h"
#include "Shader.h"
#include "UserInterface.h"

#include "StencilTest.h"

#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtc/type_ptr.hpp"
#include "STB Image/stb_image.h"

#include <iostream>

namespace Pink
{

	/*
	*
	* Forward Declarations
	*
	*/
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouse_callback(GLFWwindow* window, double xPosition, double yPosition);
	void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

	/*
	*
	*  Variables
	*
	*/
	Camera camera(glm::vec3(0.0f, 1.0f, 10.0f));
	float lastMouseXPosition = 0.0f;
	float lastMouseYPosition = 0.0f;
	bool firstMousePosition = true;

	float deltaTime = 0.0f;
	float lastFrameTime = 0.0f;

	bool wireframeMode = false;
	bool enableCursor = false;

	/*
	*
	* Constructor & Destructor
	*
	*/
	Renderer::Renderer(int width, int height) :
		windowWidth(width), windowHeight(height)
	{
		lastMouseXPosition = static_cast<float>(width) * 0.5f;
		lastMouseYPosition = static_cast<float>(height) * 0.5f;

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(width, height, "Learn OpenGL", NULL, NULL);

		if (window == nullptr)
		{
			std::cout << "Failed to create GLFW window." << std::endl;

			glfwTerminate();

			throw;
		}

		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetKeyCallback(window, key_callback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glfwSetInputMode(window, GLFW_CURSOR, enableCursor ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD." << std::endl;

			throw;
		}

		settings = new Settings(maximumVertexAttributes());
		userInterface = new UserInterface(settings, window);
	}

	Renderer::~Renderer()
	{
		glfwTerminate();

		delete settings;
		delete userInterface;
	}

	/*
	*
	* Private Methods
	*
	*/
	GLuint Pink::Renderer::loadTexture(char const* path)
	{
		GLuint textureID;
		glGenTextures(1, &textureID);

		int width;
		int height;
		int numberOfComponents;

		unsigned char* data = stbi_load(path, &width, &height, &numberOfComponents, 0);

		if (data)
		{
			GLenum format = GL_RED;

			if (numberOfComponents == 3)
			{
				format = GL_RGB;
			}
			else if (numberOfComponents == 4)
			{
				format = GL_RGBA;
			}

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else
		{
			std::cout << "Failed to load texture at path: " << path << std::endl;
		}

		stbi_image_free(data);

		return textureID;
	}

	void Renderer::processInput()
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}

		const float cameraSpeed = 2.5f * deltaTime;

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			camera.processKeyboard(FORWARD, deltaTime);
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			camera.processKeyboard(BACKWARD, deltaTime);
		}

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			camera.processKeyboard(LEFT, deltaTime);
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			camera.processKeyboard(RIGHT, deltaTime);
		}
	}

	void Renderer::processUI()
	{
		// Do nothing for now.
	}

	/*
	*
	* Public Methods
	*
	*/
	int Renderer::maximumVertexAttributes()
	{
		int numberOfAttributes;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &numberOfAttributes);

		return numberOfAttributes;
	}

	void Renderer::render()
	{
		// Have STBI flip images when loading from file.
		stbi_set_flip_vertically_on_load(true);

		// Clear color for the frame buffer.
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		// Enable OpenGL depth testing.
		glEnable(GL_DEPTH_TEST);

		Scene* scene = new StencilTest();

		// FPS and frame time calculations.
		double lastTime = glfwGetTime();
		int numberOfFrames = 0;

		while (!glfwWindowShouldClose(window))
		{
			// Set up delta time for smooth camera or animation movements.
			float currentFrameTime = static_cast<float>(glfwGetTime());
			deltaTime = currentFrameTime - lastFrameTime;
			lastFrameTime = currentFrameTime;

			// Calculate FPS and frame time.
			numberOfFrames++;

			settings->fps = static_cast<int>(numberOfFrames / deltaTime);
			settings->frameTime = 1000.0f / numberOfFrames;

			if (deltaTime >= 1.0)
			{
				numberOfFrames = 0;
				lastTime += 1.0;
			}

			// Input handling.
			processInput();

			// UI updates.
			processUI();

			// Render commands.
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Tell ImGui we're rendering a new frame and to style the UI.
			userInterface->newFrame();
			userInterface->style();

			// Model, View, and Projection matrices.
			float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 view = camera.getViewMatrix();
			glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), aspectRatio, 0.1f, 100.0f);

			scene->draw(model, view, projection);

			// After rendering our frame in OpenGL, create our ImGui UI.
			userInterface->draw();

			// Render ImGui UI.
			userInterface->render();

			// Check and call events, swap buffers.
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		delete scene;
	}

	/*
*
* Callback Functions
*
*/
	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_M && action == GLFW_PRESS)
		{
			wireframeMode = !wireframeMode;

			// Toggle wireframe mode.
			glPolygonMode(GL_FRONT_AND_BACK, wireframeMode ? GL_LINE : GL_FILL);
		}

		if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
		{
			enableCursor = !enableCursor;

			glfwSetInputMode(window, GLFW_CURSOR, enableCursor ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
		}
	}

	void mouse_callback(GLFWwindow* window, double xPosition, double yPosition)
	{
		float xPositionFloat = static_cast<float>(xPosition);
		float yPositionFloat = static_cast<float>(yPosition);

		if (firstMousePosition)
		{
			lastMouseXPosition = xPositionFloat;
			lastMouseYPosition = yPositionFloat;

			firstMousePosition = false;
		}

		float xOffset = xPositionFloat - lastMouseXPosition;
		float yOffset = lastMouseYPosition - yPositionFloat;

		lastMouseXPosition = xPositionFloat;
		lastMouseYPosition = yPositionFloat;

		if (!enableCursor)
		{
			camera.processMouseMovement(xOffset, yOffset);
		}
	}

	void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
	{
		if (enableCursor)
		{
			camera.processMouseScroll(static_cast<float>(yOffset));
		}
	}

}