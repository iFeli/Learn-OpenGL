#ifndef MESH_H
#define MESH_H

#include <GLM/glm.hpp>

#include <string>
#include <vector>

#include "Shader.h"

namespace Pink
{
	struct Vertex 
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 textureCoordinates;
	};

	struct Texture
	{
		enum Type {
			DIFFUSE,
			SPECULAR
		};

		unsigned int id;
		std::string type;
		std::string path;
	};

	class Mesh
	{
	public:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

		void draw(Shader& shader);

	private:
		unsigned int vao;
		unsigned int vbo;
		unsigned int ebo;

		void setUpMesh();
	};
}

#endif