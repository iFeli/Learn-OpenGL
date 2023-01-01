#pragma once

/*
* GLAD is included before GLFW as it includes the required OpenGL headers that GLFW(and others) use.
*/
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

namespace Pink
{
	class Renderer;
}

class Pink::Renderer
{
private:
	int width;
	int height;
	bool wireframeMode;

	GLFWwindow* glfwWindow = nullptr;

	unsigned int createVertexShader();
	unsigned int createFragmentShader();
	unsigned int createShaderProgram();

	void processInput(GLFWwindow* window);

public:
	Renderer(int width, int height);

	void render();
};