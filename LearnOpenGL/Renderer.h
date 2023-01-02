#pragma once

/*
* GLAD is included before GLFW as it includes the required OpenGL headers that GLFW(and others) use.
*/
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include "Settings.h"
#include "UserInterface.h"

namespace Pink
{
	class Renderer;
}

class Pink::Renderer
{
private:
	Settings* settings;
	UserInterface* userInterface = nullptr;
	GLFWwindow* window = nullptr;

	int width;
	int height;
	bool wireframeMode = false;
	
	unsigned int createVertexShader();
	unsigned int createFragmentShader();
	unsigned int createShaderProgram();

	void processInput();
	void processUI(const unsigned int shaderProgram);

public:
	Renderer(int width, int height);
	~Renderer();

	int maximumVertexAttributes();
	void render();
};