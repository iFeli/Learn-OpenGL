#ifndef MODEL_LOADING_H
#define MODEL_LOADING_H

#include "GLM/gtc/type_ptr.hpp"

#include "Model.h"
#include "Scene.h"
#include "Shader.h"

namespace Pink
{

	class ModelLoading : public Scene
	{
	private:
		Model backpack;
		Shader modelShader;

	public:
		ModelLoading() :
			backpack(Model("Resource Files/Models/Backpack/backpack.obj")),
			modelShader(Shader("Resource Files/Shaders/Model.vert", "Resource Files/Shaders/Model.frag"))
		{
			// Shader set up.
			modelShader.use();
			modelShader.setInteger("material.diffuse", 0);
			modelShader.setInteger("material.specular", 1);
			modelShader.setFloat("material.shininess", 32.0f);
		}

		void draw(const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection)
		{
			//
			// Draw the backpack loaded from a file.
			//
			modelShader.use();
			modelShader.setMatrix4("model", glm::value_ptr(model));
			modelShader.setMatrix4("view", glm::value_ptr(view));
			modelShader.setMatrix4("projection", glm::value_ptr(projection));

			backpack.draw(modelShader);
		}
	};

}

#endif