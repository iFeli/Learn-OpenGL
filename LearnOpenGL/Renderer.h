#pragma once

/*
* GLAD is included before GLFW as it includes the required OpenGL headers that GLFW(and others) use.
*/
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace Pink
{
	class Window;
}

class Pink::Window
{
private:
	int width;
	int height;

	GLFWwindow* glfwWindow = nullptr;

	void processInput(GLFWwindow* window);

public:
	Window(int width, int height);

	void render();
};