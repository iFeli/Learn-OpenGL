#pragma once

#include <GLFW/glfw3.h>

#include "Settings.h"

namespace Pink
{
	class UserInterface;
}

class Pink::UserInterface
{
private:
	Settings* const settings;
	GLFWwindow* const window;

	void drawInfo();
	void drawOpenGLSettings();

public:
	UserInterface(Settings* const settings, GLFWwindow* const window);
	~UserInterface();

	void draw();
	void newFrame();
	void render();
	void style();
};