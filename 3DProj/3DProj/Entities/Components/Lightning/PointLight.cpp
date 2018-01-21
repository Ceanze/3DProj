#include "PointLight.h"

#include "../../Entity.h"

#include "../../../Shading/Deferred Rendering/PhongLS.h"

PointLight::PointLight(const float& radius, const float& intensity, const glm::vec3& color, PhongLS* phongShader) : LightComponent(phongShader)
{
	this->data.colorIntensity = glm::vec4(color, intensity);	
	this->radius = radius;
}


PointLight::~PointLight()
{
}

void PointLight::init()
{
	this->data.positionRadius = glm::vec4(this->getEntity()->getWorldTransform().getTranslation(), this->radius);
	this->getShader()->addPointLight(&this->data);
}

void PointLight::update()
{
	this->data.positionRadius = glm::vec4(this->getEntity()->getWorldTransform().getTranslation(), this->radius);
}
