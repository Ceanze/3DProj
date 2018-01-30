#include "Movement.h"

#include "..\..\Entity.h"
#include "glfw3.h"
#include "../../../Core/Config.h"



Movement::Movement(float speed, unsigned forward, unsigned left, unsigned backward, unsigned right)
{
	this->speed = speed;
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
	//glm::vec3 direction;
	//if(this->getEntity()->getComponent() != nullptr)
	//	direction = dynamic_cast<Camera*>(this->getEntity()->getComponent())->getDirection(); //.getDirection();
	glm::vec3 forward = this->getEntity()->getLocalTransform().getDirection();
	glm::vec3 right = glm::normalize(glm::cross(forward, GLOBAL_UP_VECTOR));

	// -------------------------------- Move position --------------------------------
	if (glfwGetKey(display->getWindowPtr(), this->forward) == GLFW_PRESS)
		this->getEntity()->getWorldTransform().setTranslation(this->getEntity()->getWorldTransform().getTranslation() + this->speed*forward*dt);// glm::vec3(0.0, 0.0, this->speed)*dt);
	if (glfwGetKey(display->getWindowPtr(), this->backward) == GLFW_PRESS)
		this->getEntity()->getWorldTransform().setTranslation(this->getEntity()->getWorldTransform().getTranslation() - this->speed*forward*dt);
	if (glfwGetKey(display->getWindowPtr(), this->right) == GLFW_PRESS)
		this->getEntity()->getWorldTransform().setTranslation(this->getEntity()->getWorldTransform().getTranslation() + this->speed*right*dt);
	if (glfwGetKey(display->getWindowPtr(), this->left) == GLFW_PRESS)
		this->getEntity()->getWorldTransform().setTranslation(this->getEntity()->getWorldTransform().getTranslation() - this->speed*right*dt);
	// -------------------------------------------------------------------------------
}