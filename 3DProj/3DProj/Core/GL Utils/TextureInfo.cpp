#include "TextureInfo.h"

#include "../../Stb/stb_image.h"

TextureInfo::TextureInfo() : 
	data(nullptr), 
	width(0), 
	height(0), 
	dataType(GL_UNSIGNED_BYTE), 
	externalFormat(GL_RGBA), 
	internalFormat(GL_RGBA)
{
}

TextureInfo::TextureInfo(unsigned int width, unsigned int height, void * data, GLuint dataType, GLuint externalFormat, GLuint internalFormat)
{
	this->width = width;
	this->height = height;
	this->data = data;
	this->dataType = dataType;
	this->externalFormat = externalFormat;
	this->internalFormat = internalFormat;
}
