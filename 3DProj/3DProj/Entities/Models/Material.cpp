#include "Material.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../Stb/stb_image.h"

#include "../../Error.h"
#include "../../Core/Config.h"

#include "../../Core/ResourceManager.h"

Material::Material(bool useNormalMap)
{
	this->ka = { 0.2f, 0.2f, 0.2f };
	this->kd = { 0.6f, 0.6f, 0.6f };
	this->ks = { 1.0f, 1.0f, 1.0f };
	this->glowColor = {0.0f, 0.0f, 0.0f};
	this->ns = 90.0f;

	this->texture = nullptr;
	this->hasTexture = false;

	this->normalMap = nullptr;
	if (useNormalMap)
		loadDefaultNormalMap();
}

Material::~Material()
{
	delete this->texture;
	delete this->normalMap;
}

void Material::loadTexture(const std::string & path)
{
	this->hasTexture = true;
	char hasFailed = ResourceManager::loadTexture(path, &this->texture);
	if (hasFailed == TEXTURE_FAILED)
		Error::printError("Failed to load texture: " + path);
	else if (hasFailed == TEXTURE_SUCCEEDED)
		Error::print("[Loaded texture]:", path);
}

void Material::loadNormalMap(const std::string & path)
{
	char hasFailed = ResourceManager::loadTexture(path, &this->normalMap);
	if (hasFailed == TEXTURE_FAILED)
		Error::printError("Failed to load normal map: " + path);
	else if (hasFailed == TEXTURE_SUCCEEDED)
		Error::print("[Loaded normal map]:", path);
}

void Material::loadDefaultNormalMap()
{
	char hasFailed = ResourceManager::loadTexture(DEFAULT_NORMAL_MAP_PATH, &this->normalMap);
	if (hasFailed == TEXTURE_FAILED)
		Error::printError("Failed to load default normal map: " + std::string(DEFAULT_NORMAL_MAP_PATH));
	else if(hasFailed == TEXTURE_SUCCEEDED)
		Error::print("[Loaded default normal map]:", std::string(DEFAULT_NORMAL_MAP_PATH));
}
