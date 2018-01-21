#include "ResourceManager.h"

#include "../Stb/stb_image.h"
#include "Config.h"

std::unordered_map<std::string, ResourceManager::TextureData> ResourceManager::texturesData = std::unordered_map<std::string, ResourceManager::TextureData>();
std::vector<Material*> ResourceManager::materialsData = std::vector<Material*>();

char ResourceManager::loadTexture(const std::string & path, Texture** texture)
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

void ResourceManager::addMaterial(Material * material)
{
	materialsData.push_back(material);
}

void ResourceManager::addMaterials(const std::vector<Material*>& materials)
{
	for (Material* m : materials)
		materialsData.push_back(m);
}

void ResourceManager::deleteResources()
{
	for (std::pair<std::string, TextureData> p : texturesData)
		stbi_image_free(p.second.data);

	for (Material* m : materialsData)
		delete m;
}

bool ResourceManager::isTextureLoaded(const std::string & path)
{
	if(texturesData.find(path) != texturesData.end())
		return true;
	else
		return false;
}
