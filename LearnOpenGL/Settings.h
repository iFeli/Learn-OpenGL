#pragma once

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
	
	ImColor fillColor = ImColor(0.0f, 0.45f, 0.85f, 1.0f);
	bool wireframeMode = false;
};