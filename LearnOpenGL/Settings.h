#pragma once

namespace Pink
{
	class Settings;
}

class Pink::Settings
{
public:
	Settings(int maximumVertexAttributes);

	const int maximumVertexAttributes;
	bool wireframeMode = false;
};