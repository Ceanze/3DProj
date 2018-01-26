#include "Texture.h"

Texture::Texture()
{
	init(1, 16, 16, nullptr);
}

Texture::Texture(const std::vector<TextureInfo>& textureInfos)
{
	this->textureIndex = 0;
	this->numTextures = textureInfos.size();
	this->textures = nullptr;
	for(unsigned int i = 0; i < textureInfos.size(); i++)
		this->textureInfos.push_back(textureInfos[i]);
	createTextures();
}

Texture::Texture(unsigned int width, unsigned int height, unsigned int numTextures)
{
	init(numTextures, width, height, nullptr);
}

Texture::Texture(unsigned int width, unsigned int height, void* data, unsigned int numTextures)
{
	init(numTextures, width, height, data);
}

Texture::Texture(unsigned int width, unsigned int height, void * data, GLuint dataType, GLuint externalFormat, GLuint internalFormat, unsigned int numTextures)
{
	init(numTextures, width, height, data, dataType, externalFormat, internalFormat);
}

Texture::~Texture()
{
	glDeleteTextures(this->numTextures, this->textures);
	delete[] this->textures;
}

void Texture::recreate(unsigned int width, unsigned int height, void * data)
{
	for (unsigned int i = 0; i < numTextures; i++)
	{
		this->textureInfos[i].data = data;
		this->textureInfos[i].width = width;
		this->textureInfos[i].height = height;

		glBindTexture(GL_TEXTURE_2D, this->textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, this->textureInfos[i].internalFormat, width, height, 0, this->textureInfos[i].externalFormat, this->textureInfos[i].dataType, this->textureInfos[i].data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Texture::setTextureData(unsigned int index, unsigned int width, unsigned int height, void * data, GLuint dataType, GLuint externalFormat, GLuint internalFormat)
{
	TextureInfo& textureInfo = this->textureInfos[index];
	textureInfo.data = data;
	textureInfo.width = width;
	textureInfo.height = height;
	textureInfo.dataType = dataType;
	textureInfo.externalFormat = externalFormat;
	textureInfo.internalFormat = internalFormat;

	glBindTexture(GL_TEXTURE_2D, this->textures[index]);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, externalFormat, dataType, data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind(unsigned int index)
{
	glBindTexture(GL_TEXTURE_2D, this->textures[index]);
}

void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture::getTexture(unsigned int index) const
{
	return this->textures[index];
}

GLuint * Texture::getTextures() const
{
	return this->textures;
}

unsigned int Texture::getNumTextures() const
{
	return this->numTextures;
}

unsigned int Texture::getWidth(unsigned int index) const
{
	return this->textureInfos[index].width;
}

unsigned int Texture::getHeight(unsigned int index) const
{
	return this->textureInfos[index].height;
}

const std::vector<TextureInfo>& Texture::getTextureData() const
{
	return this->textureInfos;
}

void Texture::init(unsigned int numTextures, unsigned int width, unsigned int height, void * data, GLuint dataType, GLuint externalFormat, GLuint internalFormat)
{
	this->textureIndex = 0;
	this->numTextures = numTextures;
	for (unsigned int i = 0; i < numTextures; i++)
		this->textureInfos.push_back(TextureInfo(width, height, data, dataType, externalFormat, internalFormat));

	createTextures();
}

void Texture::createTextures()
{
	this->textures = new GLuint[this->textureInfos.size()];
	glGenTextures(this->textureInfos.size(), this->textures);

	for (unsigned int i = 0; i < this->textureInfos.size(); i++)
		createTexture(this->textureInfos[i].width, this->textureInfos[i].height, this->textureInfos[i].data, this->textureInfos[i].dataType, this->textureInfos[i].externalFormat, this->textureInfos[i].internalFormat);
}

void Texture::createTexture(unsigned int width, unsigned int height, void* data, GLuint dataType, GLuint externalFormat, GLuint internalFormat)
{
	glBindTexture(GL_TEXTURE_2D, this->textures[this->textureIndex++]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, externalFormat, dataType, data);

	glBindTexture(GL_TEXTURE_2D, 0);
}
