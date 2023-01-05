#include "Renderer.h"

#include "Shader.h"
#include "UserInterface.h"

#include <iostream>

/*
* 
* Forward Declarations
* 
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

/*
* 
* Constructor & Destructor
* 
*/
Pink::Renderer::Renderer(int width, int height) :
	width(width), height(height)
{
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
	float vertices[] = {
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
	};

	unsigned int indices[] = {
		0, 1, 2
	};

	Shader shader = Shader("Shader.vert", "Shader.frag");

	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;

	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));
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

	while (!glfwWindowShouldClose(window))
	{
		// Input.
		processInput();

		// UI updates.
		processUI();

		// Render commands.
		glClear(GL_COLOR_BUFFER_BIT);

		// Tell ImGui we're rendering a new frame and to style the UI.
		userInterface->newFrame();
		userInterface->style();

		// Draw commands.
		shader.use();
		shader.setFloat("xOffset", 0.5f);

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// After rendering our frame in OpenGL, create our ImGui UI.
		userInterface->draw();

		// Render ImGui UI.
		userInterface->render();

		// Check and call events, swap buffers.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
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