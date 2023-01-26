#include "ModelLoading.h"

#include "GLM/gtc/type_ptr.hpp"

namespace Pink
{

	/*
	*
	* Constructor & Destructor
	*
	*/
	ModelLoading::ModelLoading() :
		backpack(Model("Resource Files/Models/Backpack/backpack.obj")),
		modelShader(Shader("Resource Files/Shaders/Model.vert", "Resource Files/Shaders/Model.frag"))
	{
		// Shader set up.
		modelShader.use();
		modelShader.setInteger("material.diffuse", 0);
		modelShader.setInteger("material.specular", 1);
		modelShader.setFloat("material.shininess", 32.0f);
	}
	
	/*
	*
	* Public Methods
	*
	*/
	void ModelLoading::draw(const Camera& camera, const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection)
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

}
