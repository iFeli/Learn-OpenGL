#ifndef VERTEX_H
#define VERTEX_H

#include <GLM/glm.hpp>

namespace Pink
{

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 textureCoordinates;
	};

}

#endif