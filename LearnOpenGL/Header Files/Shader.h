#ifndef SHADER_H
#define SHADER_H

#include <string>

namespace Pink
{
	class Shader;
}

class Pink::Shader
{
public: 
	unsigned int id;

	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
	~Shader();

	void use();

	void setBool(const std::string& name, bool value) const;
	void setFloat(const std::string& name, float value) const;
	void setInt(const std::string& name, int value) const;
	void setMatrix4Float(const std::string& name, float* value) const;
};

#endif