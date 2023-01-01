#pragma once

/*
* GLAD is included before GLFW as it includes the required OpenGL headers that GLFW(and others) use.
*/
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include "UserInterface.h"

namespace Pink
{
	class Renderer;
}

class Pink::Renderer
{
private:
	UserInterface* userInterface;
	GLFWwindow* window;

	int width;
	int height;
	bool wireframeMode;
	
	unsigned int createVertexShader();
	unsigned int createFragmentShader();
	unsigned int createShaderProgram();

	void processInput();
	void processUI();

public:
	Renderer(int width, int height);
	~Renderer();

	void render();
};