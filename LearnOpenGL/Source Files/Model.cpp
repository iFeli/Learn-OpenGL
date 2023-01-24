#include "Model.h"

#include <GLAD/glad.h> 
#include <STB Image/stb_image.h>

#include <iostream>
#include <string>

#include "Texture.h"

namespace Pink
{

	/*
	*
	* Constructor & Destructor
	*
	*/
	Model::Model(const char* path)	
	{
		loadModel(path);
	}

	Model::~Model()
	{
		for (Mesh mesh : meshes)
		{
			for (Texture texture : mesh.textures)
			{
				glDeleteTextures(1, &texture.id);
			}
		}
	}

	/*
	*
	* Private Methods
	*
	*/
	std::vector<Texture> Model::loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName)
	{
		std::vector<Texture> textures;

		for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
		{
			aiString path;
			material->GetTexture(type, i, &path);

			bool skip = false;

			for (unsigned int j = 0; j < texturesLoaded.size(); j++)
			{
				if (std::strcmp(texturesLoaded[j].path.data(), path.C_Str()) == 0)
				{
					textures.push_back(texturesLoaded[j]);

					skip = true;

					break;
				}
			}

			if (!skip)
			{
				Texture texture;
				texture.id = textureFromFile(path.C_Str(), this->directory);
				texture.type = typeName;
				texture.path = path.C_Str();

				textures.push_back(texture);
			}
		}

		return textures;
	}

	void Model::loadModel(std::string path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR:ASSIMP::" << importer.GetErrorString() << std::endl;

			return;
		}

		directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);
	}
	
	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;

			glm::vec3 vector3 = glm::vec3(0.0f);
			
			vector3.x = mesh->mVertices[i].x;
			vector3.y = mesh->mVertices[i].y;
			vector3.z = mesh->mVertices[i].z;

			vertex.position = vector3;

			if (mesh->HasNormals())
			{
				vector3.x = mesh->mNormals[i].x;
				vector3.y = mesh->mNormals[i].y;
				vector3.z = mesh->mNormals[i].z;

				vertex.normal = vector3;
			}

			if (mesh->mTextureCoords[0])
			{
				glm::vec2 vector2 = glm::vec2(0.0f);
				vector2.x = mesh->mTextureCoords[0][i].x;
				vector2.y = mesh->mTextureCoords[0][i].y;

				vertex.textureCoordinates = vector2;
			}
			else
			{
				vertex.textureCoordinates = glm::vec2(0.0f);
			}

			vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		return Mesh(vertices, indices, textures);
	}

	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}
	
	GLuint Model::textureFromFile(const char* path, const std::string& directory, bool gamma)
	{
		std::string filename = std::string(path);
		filename = directory + '/' + filename;

		GLuint textureID;
		glGenTextures(1, &textureID);

		int width;
		int height;
		int numberOfComponents;

		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &numberOfComponents, 0);

		if (data)
		{
			GLenum format = GL_RGBA;

			if (numberOfComponents == 1)
			{
				format = GL_RED;
			}
			else if (numberOfComponents == 3)
			{
				format = GL_RGB;
			}

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;

			stbi_image_free(data);
		}

		return textureID;
	}

	/*
	*
	* Public Methods
	*
	*/
	void Model::draw(Shader& shader)
	{
		for (unsigned int i = 0; i < meshes.size(); i++)
		{
			meshes[i].draw(shader);
		}
	}
}