#ifndef MODEL_H
#define MODEL_H

#include <Assimp/Importer.hpp>
#include <Assimp/postprocess.h>
#include <Assimp/scene.h>

#include <string>
#include <vector>

#include "Mesh.h"
#include "Shader.h"

namespace Pink
{

	class Model
	{
	public:
		Model(const char* path);

		void draw(Shader& shader);

	private:
		std::vector<Mesh> meshes;
		std::string directory;
		std::vector<Texture> texturesLoaded;

		std::vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName);
		void loadModel(std::string path);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		void processNode(aiNode* node, const aiScene* scene);
		unsigned int textureFromFile(const char* path, const std::string& directory, bool gamma = false);
	};

}

#endif