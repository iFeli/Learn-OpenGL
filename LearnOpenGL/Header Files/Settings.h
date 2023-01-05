#ifndef SETTINGS_H
#define SETTINGS_H

#include <ImGui/imgui_impl_opengl3.h>

namespace Pink
{
	class Settings;
}

class Pink::Settings
{
public:
	Settings(int maximumVertexAttributes);

	const int maximumVertexAttributes;
	
	ImColor clearColor = ImColor(0.75f, 0.1f, 0.5f, 1.0f);
	bool wireframeMode = false;
};

#endif