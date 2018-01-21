#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <unordered_map>

#include "GL Utils\Texture.h"
#include "../Entities/Models/Material.h"

class ResourceManager
{
public:
	// Load a texture once even if it is called more times than once. Returns TEXTURE_FAILED if failed, TEXTURE_SUCCEEDED if succeeded and TEXTURE_PASSED if already loaded.
	static char loadTexture(const std::string& path, Texture** texture);

	static void addMaterial(Material* material);
	static void addMaterials(const std::vector<Material*>& materials);

	static void deleteResources();

private:
	struct TextureData 
	{
		TextureData() = default;
		TextureData(void* data, unsigned int width, unsigned int height)
		{
			this->data = data;
			this->width = width;
			this->height = height;
		}
		void* data;
		unsigned int width;
		unsigned int height;
	};
	static bool isTextureLoaded(const std::string& path);

	static std::unordered_map<std::string, TextureData> texturesData;
	static std::vector<Material*> materialsData;
};

#endif
