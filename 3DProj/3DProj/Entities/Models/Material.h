#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm.hpp>

class Material
{
public:
	Material();
	~Material();

	glm::vec3 ka;
	glm::vec3 kd;
	glm::vec3 ks;
	float ns;

};

#endif
