#ifndef SETTINGS_H
#define SETTINGS_H

#include <ImGui/imgui_impl_opengl3.h>

namespace Pink
{

	class Settings
	{
	public:
		Settings(int maximumVertexAttributes);
		
		const int maximumVertexAttributes;

		int fps;
		float frameTime;
		bool resetCamera;
		bool wireframe;
	};

}

#endif