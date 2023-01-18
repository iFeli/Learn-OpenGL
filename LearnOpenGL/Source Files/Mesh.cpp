#include "Mesh.h"

#include <GLAD/glad.h>

#include <string>

namespace Pink
{

	/*
	*
	* Constructor & Destructor
	*
	*/
	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) :
		vertices(vertices), indices(indices), textures(textures)
	{
		setUpMesh();
	}

	/*
	*
	* Private Methods
	*
	*/
	void Mesh::setUpMesh()
	{
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// Vertex positions.
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		// Vertex normals.
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		// Vertex texture coordinates.
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinates));

		glBindVertexArray(0);
	}

	/*
	*
	* Public Methods
	*
	*/
	void Mesh::draw(Shader& shader)
	{
		unsigned int diffuseTextureNumber = 1;
		unsigned int specularTextureNumber = 1;

		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);

			std::string number;
			std::string name = textures[i].type;

			if (name == "texture_diffuse")
			{
				number = std::to_string(diffuseTextureNumber++);
			}
			else if (name == "texture_specular")
			{
				number = std::to_string(specularTextureNumber++);
			}

			shader.setFloat(("material." + name + number).c_str(), static_cast<float>(i));

			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}

		glActiveTexture(GL_TEXTURE0);

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}