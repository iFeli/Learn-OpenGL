#ifndef MODEL_LOADING_H
#define MODEL_LOADING_H

#include "Model.h"
#include "Scene.h"
#include "Shader.h"

namespace Pink
{

	class ModelLoading : public Scene
	{
	public:
		ModelLoading();

		void draw(const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection);
		
	private:
		Model backpack;
		Shader modelShader;
	};

}

#endif