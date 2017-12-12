#include "EngineCore.h"

EngineCore::EngineCore()
{
}


EngineCore::~EngineCore()
{
}

void EngineCore::init()
{


	while (!glfwWindowShouldClose(window))
	{
		this->update(1.0f);
		this->render();
	}

}

void EngineCore::update(const float & dt)
{
}

void EngineCore::render()
{

}

