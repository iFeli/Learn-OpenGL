#ifndef RENDERER_H
#define RENDERER_H
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

	int windowWidth;
	int windowHeight;

	void processInput();
	void processUI();

public:
	Renderer(int width, int height);
	~Renderer();

	int maximumVertexAttributes();
	void render();
};

#endif