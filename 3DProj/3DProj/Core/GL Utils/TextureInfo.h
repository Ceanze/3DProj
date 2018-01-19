#ifndef TEXTURE_INFO_H
#define TEXTURE_INFO_H

#include <glew.h>

class TextureInfo
{
public:
	TextureInfo();
	TextureInfo(unsigned int width, unsigned int height, void* data, GLuint dataType, GLuint externalFormat, GLuint internalFormat);

	void* data;
	unsigned int width;
	unsigned int height;
	GLuint dataType;
	GLuint externalFormat;
	GLuint internalFormat;
};

#endif
