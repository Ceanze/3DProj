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
