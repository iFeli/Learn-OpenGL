#ifndef STENCIL_TEST_H
#define STENCIL_TEST_H

#include <GLAD/glad.h>

#include "Scene.h"
#include "Shader.h"

namespace Pink
{

	class StencilTest : public Scene
	{
	public:
		StencilTest();
		~StencilTest();

		void draw(const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection);

	private:
		GLuint cubeTexture;
		GLuint cubeVAO;
		GLuint cubeVBO;
		GLuint floorTexture;
		GLuint floorVAO;
		GLuint floorVBO;

		Shader colorShader;
		Shader textureShader;
	};

}

#endif