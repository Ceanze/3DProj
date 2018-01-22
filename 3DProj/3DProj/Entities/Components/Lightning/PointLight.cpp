#include "PointLight.h"

#include "../../Entity.h"

#include "../../../Shading/Deferred Rendering/PhongLS.h"
#include "gtc\matrix_transform.hpp"

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

void PointLight::update(const float& dt)
{
	//glm::vec4 rotation = glm::vec4(this->getEntity()->getChainTransform().getTranslation(), 1.0f);
	this->data.positionRadius = glm::vec4(this->getEntity()->getChainTransform().getTranslation(), this->radius);
}
