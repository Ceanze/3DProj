#include "Movement.h"

#include "..\..\Entity.h"
#include "glfw3.h"



Movement::Movement(unsigned forward, unsigned left, unsigned backward, unsigned right)
{
	this->speed = 5;
	this->forward = forward;
	this->left = left;
	this->backward = backward;
	this->right = right;
}


Movement::~Movement()
{
}

void Movement::init()
{
}

void Movement::update(const float & dt)
{
	this->dt = dt;
}

void Movement::input(Display * display)
{
	// -------------------------------- Move position --------------------------------
	if (glfwGetKey(display->getWindowPtr(), this->forward) == GLFW_PRESS)
		this->getEntity()->getWorldTransform().setTranslation(this->getEntity()->getWorldTransform().getTranslation() + glm::vec3(0.0, 0.0, this->speed)*dt);
	if (glfwGetKey(display->getWindowPtr(), this->backward) == GLFW_PRESS)
		this->getEntity()->getWorldTransform().setTranslation(this->getEntity()->getWorldTransform().getTranslation() + glm::vec3(0.0, 0.0, -this->speed)*dt);
	if (glfwGetKey(display->getWindowPtr(), this->right) == GLFW_PRESS)
		this->getEntity()->getWorldTransform().setTranslation(this->getEntity()->getWorldTransform().getTranslation() + glm::vec3(this->speed, 0.0, 0.0)*dt);
	if (glfwGetKey(display->getWindowPtr(), this->left) == GLFW_PRESS)
		this->getEntity()->getWorldTransform().setTranslation(this->getEntity()->getWorldTransform().getTranslation() + glm::vec3(-this->speed, 0.0, 0.0)*dt);
	// -------------------------------------------------------------------------------
}