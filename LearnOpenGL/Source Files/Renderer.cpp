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
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
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

bool wireframeMode = false;

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
	glfwSetKeyCallback(window, key_callback);
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
	// Do nothing for now.
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
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};

	// Clear color for the frame buffer.
	glClearColor(0.75f, 0.1f, 0.5f, 1.0f);

	// Enable OpenGL depth testing.
	glEnable(GL_DEPTH_TEST);

	// Create the container VAO, the light VAO, and the VBO.
	unsigned int containerVAO;
	unsigned int lightVAO;
	unsigned int vbo;

	unsigned int positionAttributeIndex = 0;

	glGenVertexArrays(1, &containerVAO);
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &vbo);

	//
	// Container
	// 
	Shader lightShader = Shader("Resource Files/Shaders/Light.vert", "Resource Files/Shaders/Light.frag");

	// Set up the container VAO.
	glBindVertexArray(containerVAO);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute.
	glEnableVertexAttribArray(positionAttributeIndex);
	glVertexAttribPointer(positionAttributeIndex, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	// The vertex buffer object (VBO) can be unbound before the vertex array object (VAO) is unbound because
	// the call to glVertexAttribPointer registers the VBO as the VAO's currently bound vertex buffer object.
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// The vertex array object (VAO) is unbound in order to avoid accidentally modifying it with subsequent calls.
	glBindVertexArray(0);

	//
	// Light
	//
	Shader containerShader = Shader("Resource Files/Shaders/Container.vert", "Resource Files/Shaders/Container.frag");

	// Set up the light VAO.
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// No need to bind the data to the VBO as that was already done for the container VAO and the data remains the same.

	glEnableVertexAttribArray(positionAttributeIndex);
	glVertexAttribPointer(positionAttributeIndex, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	// The vertex buffer object (VBO) can be unbound before the vertex array object (VAO) is unbound because
	// the call to glVertexAttribPointer registers the VBO as the VAO's currently bound vertex buffer object.
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// The vertex array object (VAO) is unbound in order to avoid accidentally modifying it with subsequent calls.
	glBindVertexArray(0);

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

		//
		// Draw container cube.
		//
		containerShader.use();
		containerShader.setMatrix4("model", glm::value_ptr(model));
		containerShader.setMatrix4("view", glm::value_ptr(view));
		containerShader.setMatrix4("projection", glm::value_ptr(projection));

		containerShader.setVector3("color", 1.0f, 0.5f, 0.31f);
		containerShader.setVector3("lightColor", 1.0f, 1.0f, 1.0f);

		glBindVertexArray(containerVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//
		// Draw light cube.
		//
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.2f, 1.0f, -2.0f));
		model = glm::scale(model, glm::vec3(0.2f));

		lightShader.use();
		lightShader.setMatrix4("model", glm::value_ptr(model));
		lightShader.setMatrix4("view", glm::value_ptr(view));
		lightShader.setMatrix4("projection", glm::value_ptr(projection));

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// After rendering our frame in OpenGL, create our ImGui UI.
		userInterface->draw();

		// Render ImGui UI.
		userInterface->render();

		// Check and call events, swap buffers.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &containerVAO);
	glDeleteVertexArrays(1, &lightVAO);
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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		wireframeMode = !wireframeMode;

		// Toggle wireframe mode.
		glPolygonMode(GL_FRONT_AND_BACK, wireframeMode ? GL_LINE : GL_FILL);
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

	camera.processMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.processMouseScroll(static_cast<float>(yOffset));
}