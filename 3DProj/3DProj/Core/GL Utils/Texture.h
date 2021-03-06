#ifndef TEXTURE_H
#define TEXTURE_H

#include <glew.h>
#include <vector>

#include "TextureInfo.h"

class Texture
{
public:
	Texture();
	Texture(const std::vector<TextureInfo>& textureInfos);
	Texture(unsigned int width, unsigned int height, unsigned int numTextures = 1);
	Texture(unsigned int width, unsigned int height, void* data, unsigned int numTextures = 1);
	Texture(unsigned int width, unsigned int height, void* data, GLuint dataType, GLuint externalFormat = GL_RGBA, GLuint internalFormat = GL_RGBA, unsigned int numTextures = 1);
	virtual ~Texture();

	void recreate(unsigned int width, unsigned int height, void* data = nullptr);

	void setTextureData(unsigned int index, unsigned int width, unsigned int height, void* data, GLuint dataType = GL_UNSIGNED_BYTE, GLuint externalFormat = GL_RGBA, GLuint internalFormat = GL_RGBA);

	void bind(unsigned int index = 0);
	void unbind();

	GLuint getTexture(unsigned int index = 0) const;
	GLuint * getTextures() const;
	unsigned int getNumTextures() const;
	unsigned int getWidth(unsigned int index = 0) const;
	unsigned int getHeight(unsigned int index = 0) const;

	const std::vector<TextureInfo>& getTextureData() const;

private:
	void init(unsigned int numTextures, unsigned int width, unsigned int height, void* data, GLuint dataType = GL_UNSIGNED_BYTE, GLuint externalFormat = GL_RGBA, GLuint internalFormat = GL_RGBA);

	void createTextures();
	void createTexture(unsigned int width, unsigned int height, void* data, GLuint dataType, GLuint externalFormat, GLuint internalFormat);

	GLuint* textures;
	std::vector<TextureInfo> textureInfos;
	unsigned int numTextures;
	unsigned int textureIndex;
};

#endif
