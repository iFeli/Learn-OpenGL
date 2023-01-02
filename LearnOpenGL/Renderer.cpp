#include "Renderer.h"

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
unsigned int Pink::Renderer::createVertexShader()
{
	const char* vertexShaderSource = 
		"#version 330 core"
		"\n"
		"layout (location = 0) in vec3 position;"
		"\n"
		"void main()\n"
		"{\n"
		"    gl_Position = vec4(position.x, position.y, position.z, 1.0f);\n"
		"}\0";

	const unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);

		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

		return NULL;
	}

	return vertexShader;
}

unsigned int Pink::Renderer::createFragmentShader()
{
	const char* fragmentShaderSource =
		"#version 330 core"
		"\n"
		"out vec4 fragmentColor;"
		"\n"
		"void main()\n"
		"{\n"
		"    fragmentColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
		"}\0";

	const unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	int success;
	char infoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);

		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

		return NULL;
	}

	return fragmentShader;
}

unsigned int Pink::Renderer::createShaderProgram()
{
	const unsigned int vertexShader = createVertexShader();
	const unsigned int fragmentShader = createFragmentShader();

	if (vertexShader == NULL || fragmentShader == NULL)
	{
		return NULL;
	}

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);

		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

		return NULL;
	}

	return shaderProgram;
}

void Pink::Renderer::processInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void Pink::Renderer::processUI()
{
	glPolygonMode(GL_FRONT_AND_BACK, settings->wireframeMode ? GL_LINE : GL_FILL);
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
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	const unsigned int shaderProgram = createShaderProgram();

	if (shaderProgram == NULL)
	{
		std::cout << "ERROR::SHADER::PROGRAM::CREATION_FAILED" << std::endl;

		return;
	}

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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
	glEnableVertexAttribArray(0);

	// The vertex buffer object (VBO) can be unbound before the vertex array object (VAO) is unbound because
	// the call to glVertexAttribPointer registers the VBO as the VAO's currently bound vertex buffer object.
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// The vertex array object (VAO) is unbound in order to avoid accidentally modifying it with subsequent calls.
	glBindVertexArray(0);

	// The EBO should be unbound after the VAO is unbound. This is because the VAO stores the glBindBuffer calls
	// when the target is GL_ELEMET_ARRAY_BUFFER. If the EBO is unbound before the VAO, the VAO ends up without
	// an EBO configured for use.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glClearColor(0.75f, 0.1f, 0.5f, 1.0f);

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
		glUseProgram(shaderProgram);
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