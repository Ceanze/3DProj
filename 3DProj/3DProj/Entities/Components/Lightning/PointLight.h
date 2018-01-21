#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "..\Lightning\LightComponent.h"

#include "glm.hpp"

class PointLight : public LightComponent
{
public:
	PointLight(const float& radius, const float& intensity, const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f));
	virtual ~PointLight();

private:
	float intensity, radius;
	glm::vec3 color;
};

#endif