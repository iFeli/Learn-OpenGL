#pragma once

#include <GLFW/glfw3.h>

namespace Pink
{
	class UserInterface;
}

class Pink::UserInterface
{
private:
	bool wireframeMode;

public:
	UserInterface(GLFWwindow* const window);
	~UserInterface();

	bool getWireframeMode();

	void draw();
	void newFrame();
	void render();
};