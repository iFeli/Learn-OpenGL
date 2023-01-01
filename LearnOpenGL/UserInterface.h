#pragma once

#include <GLFW/glfw3.h>

namespace Pink
{
	class UserInterface;
}

class Pink::UserInterface
{
private:
	GLFWwindow* const window;

	bool wireframeMode;

	void drawInfo();
	void drawOpenGLSettings();

public:
	UserInterface(GLFWwindow* const window);
	~UserInterface();

	bool getWireframeMode();

	void draw();
	void newFrame();
	void render();
	void style();
};