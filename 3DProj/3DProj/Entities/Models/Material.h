#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <glm.hpp>

#include "../../Core/GL Utils/Texture.h"

class Material
{
public:
	Material(bool useNormalMap);
	~Material();

	glm::vec3 ka;
	glm::vec3 kd;
	glm::vec3 ks;
	glm::vec3 glowColor;
	float ns;

	bool hasTexture;

	Texture* normalMap;
	Texture* texture;

	void loadTexture(const std::string& path);
	void loadNormalMap(const std::string& path);

private:
	void loadDefaultNormalMap();

};

#endif
