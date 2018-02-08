#include "DirectionalLight.h"

#include "../../../Shading/Deferred Rendering/PhongLS.h"

DirectionalLight::DirectionalLight(const glm::vec3 & direction, const float & intensity, const glm::vec3 & color, PhongLS * phongShader) : LightComponent(phongShader)
{
	this->data.direction = glm::vec4(direction, 0.0f);
	this->data.colorIntensity = glm::vec4(color, intensity);
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::init()
{
	this->getShader()->setDirectionalLight(&this->data);
}

void DirectionalLight::update(const float & dt)
{
}

glm::vec3 DirectionalLight::getDirection() const
{
	return this->data.direction;
}

float DirectionalLight::getIntensity() const
{
	return this->data.colorIntensity.w;
}

glm::vec3 DirectionalLight::getColor()
{
	return this->data.colorIntensity;
}

void DirectionalLight::setDirection(const glm::vec3 & direction)
{
	this->data.direction = glm::vec4(direction, 0.0f);
	this->getShader()->setDirectionalLight(&this->data);
}

void DirectionalLight::setIntensity(float intensity)
{
	this->data.colorIntensity.w = intensity;
	this->getShader()->setDirectionalLight(&this->data);
}

void DirectionalLight::setColor(const glm::vec3 & color)
{
	this->data.colorIntensity.x = color.x;
	this->data.colorIntensity.y = color.y;
	this->data.colorIntensity.z = color.z;
	this->getShader()->setDirectionalLight(&this->data);
}
