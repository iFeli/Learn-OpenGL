#include "Settings.h"

namespace Pink
{

	/*
	*
	* Constructor & Destructor
	*
	*/
	Settings::Settings(int maximumVertexAttributes) :
		maximumVertexAttributes(maximumVertexAttributes),
		fps(0), 
		frameTime(0.0f),
		resetCamera(false),
		wireframe(false)
	{

	}

}