#include "Movement.h"

#include "..\..\Entity.h"
#include "glfw3.h"
#include "../../../Core/Config.h"
#include "../../../Terrain/Terrain.h"
#include "../../../Error.h"



Movement::Movement(float speed, unsigned forward, unsigned left, unsigned backward, unsigned right) //Movement without terrain
{
	this->terrain = nullptr;
	this->speed = this->normalSpeed = speed;
	this->shiftSpeed = speed * 4.f;
	this->slowSpeed = speed * 0.2f;
	this->forward = forward;
	this->left = left;
	this->backward = backward;
	this->right = right;
}

Movement::Movement(Terrain * terrain, float speed, unsigned forward, unsigned left, unsigned backward, unsigned right) //Movement with terrain
{
	this->terrain = terrain;
	this->speed = this->normalSpeed = speed;
	this->shiftSpeed = speed * 4.f;
	this->slowSpeed = speed * 0.2f;
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
	static bool terrainLock = false;
	static bool isKeyPressed = false;

	if(glfwGetKey(display->getWindowPtr(), GLFW_KEY_SPACE) != GLFW_PRESS)
	{
		if (isKeyPressed)
		{
			terrainLock = !terrainLock;
			isKeyPressed = false;
		}
	}
	else isKeyPressed = true;


	static bool ShiftLock = false;
	static bool isShiftKeyPressed = false;
	static bool altLock = false;
	static bool isAltKeyPressed = false;

	glm::vec3 forward = this->getEntity()->getLocalTransform().getDirection();
	glm::vec3 right = glm::normalize(glm::cross(forward, GLOBAL_UP_VECTOR));
	glm::vec3 position = this->getEntity()->getWorldTransform().getTranslation();

	if (glfwGetKey(display->getWindowPtr(), GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS)
	{
		if (isShiftKeyPressed)
		{
			ShiftLock = !ShiftLock;
			isShiftKeyPressed = false;
			this->speed = this->normalSpeed;
		}
	}
	else isShiftKeyPressed = true;

	if (glfwGetKey(display->getWindowPtr(), GLFW_KEY_LEFT_ALT) != GLFW_PRESS)
	{
		if (isAltKeyPressed)
		{
			altLock = !altLock;
			isAltKeyPressed = false;
			this->speed = this->normalSpeed;
		}
	}
	else isAltKeyPressed = true;

	if (terrainLock && this->terrain != nullptr) //If space has been pressed, lock the player to the terrain
	{ 
		setHeight(this->terrain->getHeight(position.x, position.z));
		forward = glm::vec3(forward.x, 0, forward.z);
		right = glm::vec3(right.x, 0, right.z);
	}

	if (isShiftKeyPressed) //If shift is being pressed, set a higher movement speed
	{
		this->speed = this->shiftSpeed;

		ShiftLock = false;
	}

	if (isAltKeyPressed)
	{
		this->speed = this->slowSpeed;
		altLock = false;
	}


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
	this->getEntity()->getWorldTransform().setTranslation(glm::vec3(position.x, height + PLAYER_HEIGHT, position.z));
}
