#include "Shader.h"

#include <GLAD/glad.h>

#include <fstream>
#include <iostream>
#include <sstream>

/*
*
* Constructor & Destructor
*
*/
Pink::Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	std::string vertexShaderString;
	std::string fragmentShaderString;

	std::ifstream vertexShaderFileStream;
	std::ifstream fragmentShaderFileStream;

	vertexShaderFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vertexShaderFileStream.open(vertexShaderPath);
		fragmentShaderFileStream.open(fragmentShaderPath);

		std::stringstream vertexShaderStringStream;
		std::stringstream fragmentShaderStringStream;

		vertexShaderStringStream << vertexShaderFileStream.rdbuf();
		fragmentShaderStringStream << fragmentShaderFileStream.rdbuf();

		vertexShaderFileStream.close();
		fragmentShaderFileStream.close();

		vertexShaderString = vertexShaderStringStream.str();
		fragmentShaderString = fragmentShaderStringStream.str();
	}
	catch (std::ifstream::failure error)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}

	const char* vertexShaderCString = vertexShaderString.c_str();
	const char* fragmentShaderCString = fragmentShaderString.c_str();

	unsigned int vertexShader;
	unsigned int fragmentShader;

	int success;
	const unsigned int infoLogLength = 512;
	char infoLog[infoLogLength];

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCString, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, infoLogLength, NULL, infoLog);

		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCString, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, infoLogLength, NULL, infoLog);

		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(id, infoLogLength, NULL, infoLog);

		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Pink::Shader::~Shader()
{
	glDeleteProgram(id);
}

/*
*
* Public Methods
*
*/
void Pink::Shader::use()
{
	glUseProgram(id);
}

void Pink::Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), static_cast<int>(value));
}

void Pink::Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Pink::Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Pink::Shader::setMatrix4Float(const std::string& name, float* value) const
{
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, value);
}