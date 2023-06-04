#include "Shader.h"

#include <GLAD/glad.h>
#include "GLM/gtc/type_ptr.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

namespace Pink
{

	/*
	*
	* Constructor & Destructor
	*
	*/
	Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
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

		GLuint vertexShader;
		GLuint fragmentShader;

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

	Shader::Shader(const char* vertexShaderPath, const char* geometryShaderPath, const char* fragmentShaderPath)
	{
		std::string vertexShaderString;
		std::string geometryShaderString;
		std::string fragmentShaderString;

		std::ifstream vertexShaderFileStream;
		std::ifstream geometryShaderFileStream;
		std::ifstream fragmentShaderFileStream;

		vertexShaderFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		geometryShaderFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentShaderFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			vertexShaderFileStream.open(vertexShaderPath);
			geometryShaderFileStream.open(geometryShaderPath);
			fragmentShaderFileStream.open(fragmentShaderPath);

			std::stringstream vertexShaderStringStream;
			std::stringstream geometryShaderStringStream;
			std::stringstream fragmentShaderStringStream;

			vertexShaderStringStream << vertexShaderFileStream.rdbuf();
			geometryShaderStringStream << geometryShaderFileStream.rdbuf();
			fragmentShaderStringStream << fragmentShaderFileStream.rdbuf();

			vertexShaderFileStream.close();
			geometryShaderFileStream.close();
			fragmentShaderFileStream.close();

			vertexShaderString = vertexShaderStringStream.str();
			geometryShaderString = geometryShaderStringStream.str();
			fragmentShaderString = fragmentShaderStringStream.str();
		}
		catch (std::ifstream::failure error)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
		}

		const char* vertexShaderCString = vertexShaderString.c_str();
		const char* geometryShaderCString = geometryShaderString.c_str();
		const char* fragmentShaderCString = fragmentShaderString.c_str();

		GLuint vertexShader;
		GLuint geometryShader;
		GLuint fragmentShader;

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

		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryShader, 1, &geometryShaderCString, NULL);
		glCompileShader(geometryShader);

		glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(geometryShader, infoLogLength, NULL, infoLog);

			std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
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
		glAttachShader(id, geometryShader);
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

	Shader::~Shader()
	{
		glDeleteProgram(id);
	}

	/*
	*
	* Public Methods
	*
	*/
	void Shader::use()
	{
		glUseProgram(id);
	}

	void Shader::setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(id, name.c_str()), static_cast<int>(value));
	}

	void Shader::setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(id, name.c_str()), value);
	}

	void Shader::setInteger(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(id, name.c_str()), value);
	}

	void Shader::setMatrix4(const std::string& name, const float* value) const
	{
		glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, value);
	}

	void Shader::setMatrix4(const std::string& name, const glm::mat4& value) const
	{
		glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::setVector2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
	}

	void Shader::setVector3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
	}

	void Shader::setVector3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
	}

}