#ifndef SCENE_H
#define SCENE_H

#include <GLM/glm.hpp>

#include "Camera.h"

namespace Pink
{

	class Scene
	{
	public:
		virtual void draw(const Camera& camera, const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection) = 0;
	};

}

#endif
