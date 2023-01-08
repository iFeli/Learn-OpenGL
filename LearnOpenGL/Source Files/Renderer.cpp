#include "Renderer.h"

#include "Camera.h"
#include "Shader.h"
#include "UserInterface.h"

#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtc/type_ptr.hpp"
#include "STB Image/stb_image.h"

#include <iostream>

/*
*
* Forward Declarations
*
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPosition, double yPosition);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

/*
* 
*  Variables
* 
*/
Pink::Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastMouseXPosition = 0.0f;
float lastMouseYPosition = 0.0f;
bool firstMousePosition = true;

float deltaTime = 0.0f;
float lastFrameTime = 0.0f;

/*
*
* Constructor & Destructor
*
*/
Pink::Renderer::Renderer(int width, int height) :
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
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD." << std::endl;

		throw;
	}

	settings = new Settings(maximumVertexAttributes());
	userInterface = new UserInterface(settings, window);
}

Pink::Renderer::~Renderer()
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
void Pink::Renderer::processInput()
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

void Pink::Renderer::processUI()
{
	// Toggle wireframe mode.
	glPolygonMode(GL_FRONT_AND_BACK, settings->wireframeMode ? GL_LINE : GL_FILL);

	// Custom clear color.
	ImColor clearColor = settings->clearColor;
	glClearColor(clearColor.Value.x, clearColor.Value.y, clearColor.Value.z, 1.0f);
}

/*
*
* Public Methods
*
*/
int Pink::Renderer::maximumVertexAttributes()
{
	int numberOfAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &numberOfAttributes);

	return numberOfAttributes;
}

void Pink::Renderer::render()
{
	// Vertices and geometry info.
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	// World space position for the cubes.
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,   0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f,  -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f,  -3.5f),
		glm::vec3(-1.7f,  3.0f,  -7.5f),
		glm::vec3(1.3f, -2.0f,  -2.5f),
		glm::vec3(1.5f,  2.0f,  -2.5f),
		glm::vec3(1.5f,  0.2f,  -1.5f),
		glm::vec3(-1.3f,  1.0f,  -1.5f),
	};

	// Enable OpenGL depth testing.
	glEnable(GL_DEPTH_TEST);

	// Create the textures and load them using STB Image.
	int textureWidth;
	int textureHeight;
	int numberOfChannels;
	unsigned char* textureData;

	unsigned int texture1;

	glGenTextures(1, &texture1);

	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	textureData = stbi_load("Resource Files/Textures/Container.jpg", &textureWidth, &textureHeight, &numberOfChannels, 0);

	if (textureData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load Container texture." << std::endl;
	}

	stbi_image_free(textureData);

	unsigned int texture2;

	glGenTextures(1, &texture2);

	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	textureData = stbi_load("Resource Files/Textures/AwesomeFace.png", &textureWidth, &textureHeight, &numberOfChannels, 0);
	stbi_set_flip_vertically_on_load(false);

	if (textureData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load AwesomeFace texture." << std::endl;
	}

	stbi_image_free(textureData);

	// Create the shader.
	Shader shader = Shader("Resource Files/Shaders/Shader.vert", "Resource Files/Shaders/Shader.frag");
	shader.use();

	shader.setInt("texture1Data", 0);
	shader.setInt("texture2Data", 1);

	// Create the VAO and VBO.
	unsigned int vao;
	unsigned int vbo;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
	glEnableVertexAttribArray(0);

	// Texture coordinate attribute.
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	// The vertex buffer object (VBO) can be unbound before the vertex array object (VAO) is unbound because
	// the call to glVertexAttribPointer registers the VBO as the VAO's currently bound vertex buffer object.
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// The vertex array object (VAO) is unbound in order to avoid accidentally modifying it with subsequent calls.
	glBindVertexArray(0);

	// The EBO should be unbound after the VAO is unbound. This is because the VAO stores the glBindBuffer calls
	// when the target is GL_ELEMET_ARRAY_BUFFER. If the EBO is unbound before the VAO, the VAO ends up without
	// an EBO configured for use.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// FPS and frame time calculations.
	double lastTime = glfwGetTime();
	int numberOfFrames = 0;

	while (!glfwWindowShouldClose(window))
	{
		float currentFrameTime = static_cast<float>(glfwGetTime());
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		// Calculate FPS and frame time.
		double currentTime = glfwGetTime();

		numberOfFrames++;

		settings->fps = int(numberOfFrames / deltaTime);
		settings->frameTime = 1000.0f / numberOfFrames;

		if (deltaTime >= 1.0)
		{
			numberOfFrames = 0;
			lastTime += 1.0;
		}

		// Input.
		processInput();

		// UI updates.
		processUI();

		// Render commands.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Tell ImGui we're rendering a new frame and to style the UI.
		userInterface->newFrame();
		userInterface->style();

		// View matrix.
		const float radius = 10.0f;
		float camX = static_cast<float>(sin(currentTime) * radius);
		float camZ = static_cast<float>(cos(currentTime) * radius);

		// View matrix.
		glm::mat4 view = camera.getViewMatrix();
		
		shader.setMatrix4Float("view", glm::value_ptr(view));

		// Projection matrix.
		float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
		glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), aspectRatio, 0.1f, 100.0f);

		shader.setMatrix4Float("projection", glm::value_ptr(projection));

		// Draw commands.
		shader.setFloat("textureMix", settings->textureMix);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glBindVertexArray(vao);

		for (unsigned int i = 0; i < 10; i++)
		{
			// Model matrix.
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);

			float angle = 10.0f * i;
			model = glm::rotate(model, glm::radians(angle) * static_cast<float>(glfwGetTime()), glm::vec3(1.0f, 0.3f, 0.5f));

			shader.setMatrix4Float("model", glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// After rendering our frame in OpenGL, create our ImGui UI.
		userInterface->draw();

		// Render ImGui UI.
		userInterface->render();

		// Check and call events, swap buffers.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteTextures(1, &texture1);
	glDeleteTextures(1, &texture2);

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
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

	camera.processMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.processMouseScroll(static_cast<float>(yOffset));
}