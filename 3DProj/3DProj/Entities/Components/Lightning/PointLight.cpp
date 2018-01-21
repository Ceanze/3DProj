#include "PointLight.h"

PointLight::PointLight(const float & radius, const float & intensity, const glm::vec3& color) : LightComponent()
{
	this->intensity = intensity;
	this->radius = radius;
	this->color = color;
}

PointLight::~PointLight()
{
}
