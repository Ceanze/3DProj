#include "LightComponent.h"

PhongLS * LightComponent::phongShader = nullptr;

LightComponent::LightComponent(PhongLS* phongShader)
{
	if(phongShader != nullptr)
		this->phongShader = phongShader;
}


LightComponent::~LightComponent()
{
}

void LightComponent::init()
{
	
}

void LightComponent::update(const float & dt)
{
}

void LightComponent::input(Display * display)
{
}

PhongLS * LightComponent::getShader()
{
	return this->phongShader;
}
