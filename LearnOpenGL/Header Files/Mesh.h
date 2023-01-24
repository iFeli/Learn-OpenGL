#ifndef MESH_H
#define MESH_H

#include <GLAD/glad.h>
#include <GLM/glm.hpp>

#include <string>
#include <vector>

#include "Shader.h"
#include "Texture.h"
#include "Vertex.h"

namespace Pink
{

	class Mesh
	{
	public:
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<Texture> textures;

		Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);

		void draw(Shader& shader);

	private:
		GLuint vao;
		GLuint vbo;
		GLuint ebo;

		void setUpMesh();
	};
}

#endif