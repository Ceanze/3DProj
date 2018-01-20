#include "TextureManager.h"

#include "../Stb/stb_image.h"
#include "Config.h"

std::unordered_map<std::string, TextureManager::TextureData> TextureManager::texturesData = std::unordered_map<std::string, TextureManager::TextureData>();

char TextureManager::loadTexture(const std::string & path, Texture** texture)
{
	int width;
	int height;
	unsigned char* data;
	if (isTextureLoaded(path) == false)
	{
		int comp; // Not used
		data = stbi_load(path.c_str(), &width, &height, &comp, 4);
		if (data == nullptr)
			return TEXTURE_FAILED;
		texturesData.insert({ path, TextureData(data, (unsigned int)width, (unsigned int)height) });

		if (*texture != nullptr)
			delete *texture;
		TextureData& textureData = texturesData[path];
		*texture = new Texture(textureData.width, textureData.height, textureData.data);

		return TEXTURE_SUCCEEDED;
	}
	else
	{
		if (*texture != nullptr)
			delete *texture;
		TextureData& textureData = texturesData[path];
		*texture = new Texture(textureData.width, textureData.height, textureData.data);

		return TEXTURE_PASSED;
	}
}

void TextureManager::deleteTextures()
{
	for (std::pair<std::string, TextureData> p : texturesData)
		stbi_image_free(p.second.data);
}

bool TextureManager::isTextureLoaded(const std::string & path)
{
	if(texturesData.find(path) != texturesData.end())
		return true;
	else
		return false;
}
