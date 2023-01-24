#ifndef TEXTURE_H
#define TEXTURE_H

#include <GLAD/glad.h>
#include <STB Image/stb_image.h>

#include <iostream>
#include <string>

namespace Pink
{

	enum TextureType {
		DIFFUSE,
		SPECULAR
	};

	struct Texture
	{
		GLuint id;
		std::string type;
		std::string path;
	};

}

#endif