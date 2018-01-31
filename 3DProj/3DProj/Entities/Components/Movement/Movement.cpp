#include "Movement.h"

#include "..\..\Entity.h"
#include "glfw3.h"
#include "../../../Core/Config.h"
#include "../../../Terrain/Terrain.h"
#include "../../../Error.h"



Movement::Movement(float speed, unsigned forward, unsigned left, unsigned backward, unsigned right)
{
	this->speed = speed;
	this->forward = forward;
	this->left = left;
	this->backward = backward;
	this->right = right;
}

Movement::Movement(Terrain * terrain, float speed, unsigned forward, unsigned left, unsigned backward, unsigned right)
{
	this->terrain = terrain;
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
	glm::vec3 forward = this->getEntity()->getLocalTransform().getDirection();
	glm::vec3 right = glm::normalize(glm::cross(forward, GLOBAL_UP_VECTOR));
	glm::vec3 position = this->getEntity()->getWorldTransform().getTranslation();

	setHeight(this->terrain->getHeight(position.x, position.z));
	//setHeight(15);


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

void Movement::setHeight(float height)
{
	glm::vec3 position = this->getEntity()->getWorldTransform().getTranslation();
	this->getEntity()->getWorldTransform().setTranslation(glm::vec3(position.x, height, position.z));
}
