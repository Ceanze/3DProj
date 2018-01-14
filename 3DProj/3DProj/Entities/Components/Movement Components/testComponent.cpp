#include "testComponent.h"

#include "..\..\Entity.h"
#include "glfw3.h"
#include "..\..\..\Core\Display.h"

testComponent::testComponent(float rotSpeed)
{
	this->rotSpeed = rotSpeed;
}


testComponent::~testComponent()
{
}

void testComponent::init()
{
	
}

void testComponent::update(const float & dt)
{
	this->getEntity()->getWorldTransform().setRotation(this->getEntity()->getWorldTransform().getRotation() + glm::vec3(0.0f, this->rotSpeed * dt, 0.0));
}

void testComponent::input(Display * display)
{
	int state = glfwGetKey(display->getWindowPtr(), GLFW_KEY_UP);
	int state1 = glfwGetKey(display->getWindowPtr(), GLFW_KEY_DOWN);
	if (state == GLFW_PRESS)
	{
		rotSpeed += 1.0;
	}
	else if (state1 == GLFW_PRESS)
	{
		rotSpeed -= 1.0;
	}

		
}
