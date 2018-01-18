#include "Camera.h"

#include "../../../Core/Display.h"
#include "..\..\Entity.h"

#include <gtc\matrix_transform.hpp>

Camera::Camera(Display * display, const glm::vec3& position, float fov, float zNear, float zFar)
{
	this->display = display;
	this->position = position;
	updateProj(fov, zNear, zFar);
	lookAt({0.0f, 0.0f, 0.0f});
}

Camera::~Camera()
{
}

void Camera::init()
{
	glfwSetCursorPos(this->display->getWindowPtr(), this->display->getWidth() / 2, this->display->getHeight() / 2);
	//glfwSetInputMode(this->display->getWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Camera::update(const float & dt)
{
	this->dt = dt;
}

void Camera::input(Display * display)
{
	double xPos, yPos, sensitivity = 0.5f;

	// --------------------------------- Move Cursor ---------------------------------
	glfwGetCursorPos(this->display->getWindowPtr(), &xPos, &yPos);
	xPos -= this->display->getWidth() / 2;
	yPos -= this->display->getHeight() / 2;
	if (xPos != 0.0 || yPos != 0.0)
	{
		this->yawPitchRoll.x += xPos*dt*sensitivity;
		this->yawPitchRoll.y -= yPos*dt*sensitivity;

		rotate(this->yawPitchRoll.x, this->yawPitchRoll.y, this->yawPitchRoll.z);
	}

	glfwSetCursorPos(this->display->getWindowPtr(), this->display->getWidth() / 2, this->display->getHeight() / 2);
	// -------------------------------------------------------------------------------

	// -------------------------------- Move position --------------------------------
	static const float CAMERA_SPEED = 10.0f;
	if (glfwGetKey(this->display->getWindowPtr(), GLFW_KEY_W) == GLFW_PRESS)
		move(this->f*CAMERA_SPEED*dt);
	if (glfwGetKey(this->display->getWindowPtr(), GLFW_KEY_S) == GLFW_PRESS)
		move(-this->f*CAMERA_SPEED*dt);
	if (glfwGetKey(this->display->getWindowPtr(), GLFW_KEY_D) == GLFW_PRESS)
		move(this->r*CAMERA_SPEED*dt);
	if (glfwGetKey(this->display->getWindowPtr(), GLFW_KEY_A) == GLFW_PRESS)
		move(-this->r*CAMERA_SPEED*dt);
	// -------------------------------------------------------------------------------
}

void Camera::lookAt(const glm::vec3 & target)
{
	this->f = glm::normalize(target - this->position);
	this->r = glm::cross(f, GLOBAL_UP_VECTOR);
	this->u = glm::cross(r, f);

	this->yawPitchRoll.x = atan2(this->f.z, this->f.x) + 3.1415f / 2.0f;
	this->yawPitchRoll.y = atan2(this->f.y, -this->f.z);
	this->yawPitchRoll.z = atan2(this->u.y, this->f.x);

	updateView(this->f, this->r, this->u, this->position);
}

void Camera::setPosition(const glm::vec3 & position)
{
	this->position = position;
	updateView(f, r, u, this->position);
}

void Camera::move(const glm::vec3 & offset)
{
	this->position += offset;
	setPosition(this->position);
}

void Camera::rotate(float yaw, float pitch, float roll)
{
	this->f.x = cos(yaw - 3.1415f / 2.0f)*cos(pitch);
	this->f.y = sin(pitch);
	this->f.z = sin(yaw - 3.1415f / 2.0f)*cos(pitch);
	this->r = glm::normalize(glm::cross(f, GLOBAL_UP_VECTOR));
	this->u = glm::cross(r, f);

	this->yawPitchRoll.x = yaw;
	this->yawPitchRoll.y = pitch;
	this->yawPitchRoll.z = roll;

	updateView(f, r, u, this->position);
}

void Camera::updateProj()
{
	updateProj(this->fov, this->zNear, this->zFar);
}

float Camera::getFOV() const
{
	return this->fov;
}

glm::vec3 Camera::getPosition()
{
	return this->position;
}

glm::mat4 Camera::getVP()
{
	return this->proj*this->view;
}

void Camera::updateProj(float fov, float zNear, float zFar)
{
	this->zNear = zNear;
	this->zFar = zFar;
	this->fov = fov;
	this->proj = glm::perspective(fov, this->display->getRatio(), zNear, zFar);
}

void Camera::updateView(const glm::vec3 & f, const glm::vec3 & r, const glm::vec3 & u, const glm::vec3 & pos)
{
	this->view = glm::mat4({ r.x, u.x, -f.x, 0.0f },
	{ r.y, u.y, -f.y, 0.0f },
	{ r.z, u.z, -f.z, 0.0f },
	{ -glm::dot(r, pos), -glm::dot(u, pos), glm::dot(f, pos), 1.0f });
}
