#include "LightComponent.h"

std::vector<LightComponent*>* LightComponent::listOfLights = nullptr;

LightComponent::LightComponent()
{
	if (this->listOfLights != nullptr)
		this->listOfLights->push_back(this);
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

void LightComponent::setListOfLights(std::vector<LightComponent*>* ptr)
{
	this->listOfLights = ptr;
}
