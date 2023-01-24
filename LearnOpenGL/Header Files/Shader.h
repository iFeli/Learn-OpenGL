#ifndef SHADER_H
#define SHADER_H

#include <GLAD/glad.h>
#include <GLM/glm.hpp>

#include <string>

namespace Pink
{

	class Shader
	{
	public:
		GLuint id;

		Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
		~Shader();

		void use();

		void setBool(const std::string& name, bool value) const;
		void setFloat(const std::string& name, float value) const;
		void setInteger(const std::string& name, int value) const;
		void setMatrix4(const std::string& name, const float* value) const;
		void setMatrix4(const std::string& name, const glm::mat4& value) const;
		void setVector3(const std::string& name, float x, float y, float z) const;
		void setVector3(const std::string& name, const glm::vec3& value) const;
	};

}

#endif