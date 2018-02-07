#include "Camera.h"

#include "../../../Core/Display.h"
#include "..\..\Entity.h"
#include "../../../Error.h"

#include <gtc\matrix_transform.hpp>

Camera::Camera(Display * display, glm::vec3 relativePosition, float fov, float zNear, float zFar)
{
	this->display = display;
	this->relativePosition = relativePosition;
	this->orthoCam = false;
	updateProj(fov, zNear, zFar);
	this->active = false;
}

Camera::Camera(Display * display, float width, float height, glm::vec3 relativePosition, float zNear, float zFar)
{
	this->display = display;
	this->width = width;
	this->height = height;
	this->relativePosition = relativePosition;
	this->orthoCam = true;
	this->fov = 0.0f;
	updateProj(fov, zNear, zFar);
	this->active = false;
}

Camera::~Camera()
{
}

void Camera::init()
{
	this->isCClicked = 0;
	this->isCPressed = false;

	setRelativePosition(this->relativePosition);
	lookAt(glm::vec3(0.0f, 0.0f, 0.0f));

	glfwSetCursorPos(this->display->getWindowPtr(), this->display->getWidth() / 2, this->display->getHeight() / 2);
	//glfwSetInputMode(this->display->getWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Camera::update(const float & dt)
{
	this->dt = dt;
	setRelativePosition(this->relativePosition);
}

void Camera::input(Display * display)
{
	if (this->active)
	{
		if (glfwGetKey(this->display->getWindowPtr(), GLFW_KEY_C) != GLFW_PRESS)
		{
			if (isCPressed)
			{
				isCPressed = false;
				isCClicked ^= 1;
				glfwSetCursorPos(this->display->getWindowPtr(), this->display->getWidth() / 2, this->display->getHeight() / 2);
			}
		}
		else isCPressed = true;

		if (isCClicked)
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

		}
	}
}

void Camera::lookAt(const glm::vec3 & target)
{
	this->f = glm::normalize(target - this->worldPosition);
	this->r = glm::cross(f, GLOBAL_UP_VECTOR);
	this->u = glm::cross(r, f);

	this->yawPitchRoll.x = atan2(this->f.z, this->f.x) + 3.1415f / 2.0f;
	this->yawPitchRoll.y = atan2(this->f.y, -this->f.z);
	this->yawPitchRoll.z = atan2(this->u.y, this->f.x);

	updateView(this->f, this->r, this->u, this->worldPosition);
	this->getEntity()->getLocalTransform().setDirection(this->f);
}

void Camera::setRelativePosition(const glm::vec3 & relativePosition)
{
	this->relativePosition = relativePosition;
	setWorldPosition();
	updateView(this->f, this->r, this->u, this->worldPosition);
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

	updateView(this->f, this->r, this->u, this->worldPosition);
	this->getEntity()->getLocalTransform().setDirection(this->f);
}

void Camera::updateProj()
{
	updateProj(this->fov, this->zNear, this->zFar);
}

void Camera::activate()
{
	this->active = true;
	glfwSetCursorPos(this->display->getWindowPtr(), this->display->getWidth() / 2, this->display->getHeight() / 2);
}

void Camera::deactivate()
{
	this->active = false;
}

void Camera::setDirection(const glm::vec3 & direction)
{
	this->f = direction;
	this->r = glm::cross(f, GLOBAL_UP_VECTOR);
	this->u = glm::cross(r, f);

	this->yawPitchRoll.x = atan2(this->f.z, this->f.x) + 3.1415f / 2.0f;
	this->yawPitchRoll.y = atan2(this->f.y, -this->f.z);
	this->yawPitchRoll.z = atan2(this->u.y, this->f.x);

	updateView(this->f, this->r, this->u, this->worldPosition);
	this->getEntity()->getLocalTransform().setDirection(this->f);
}

float Camera::getFOV() const
{
	return this->fov;
}

glm::vec3 Camera::getPosition() const
{
	return this->worldPosition;
}

glm::vec3 Camera::getDirection() const
{
	return this->f;
}

glm::vec3 Camera::getUp() const
{
	return this->u;
}

glm::vec3 Camera::getRight() const
{
	return this->r;
}

glm::mat4 Camera::getVP() const
{
	return this->proj*this->view;
}

void Camera::updateProj(float fov, float zNear, float zFar)
{
	this->zNear = zNear;
	this->zFar = zFar;
	this->fov = fov;
	if (this->orthoCam)
		this->proj = glm::ortho(-this->width / 2, this->width / 2, -this->height / 2, this->height / 2, zNear, zFar);
	else
		this->proj = glm::perspective(fov, this->display->getRatio(), zNear, zFar);
}

void Camera::updateView(const glm::vec3 & f, const glm::vec3 & r, const glm::vec3 & u, const glm::vec3 & pos)
{
	this->view = glm::mat4({ r.x, u.x, -f.x, 0.0f },
	{ r.y, u.y, -f.y, 0.0f },
	{ r.z, u.z, -f.z, 0.0f },
	{ -glm::dot(r, pos), -glm::dot(u, pos), glm::dot(f, pos), 1.0f });
}

void Camera::setLocalPositionMat()
{
	this->localPositionMat = glm::mat4(1, 0, 0, 0,
									   0, 1, 0, 0,
									   0, 0, 1, 0,
		this->relativePosition.x, this->relativePosition.y, this->relativePosition.z, 1);
}

void Camera::setWorldPosition()
{
	setLocalPositionMat();
	glm::mat4 temp = this->getEntity()->getWorldTransform().getMatrix()*this->localPositionMat;

	this->worldPosition = glm::vec3(temp[3][0], temp[3][1], temp[3][2]);
	//this->worldPosition = glm::vec3(0.0, 5.0, 10.0);
	//Error::printError("Chainmatrix" + std::to_string(this->getEntity()->getChainTransform().getMatrix()[3][0]) + ", " + std::to_string(this->getEntity()->getChainTransform().getMatrix()[3][1]) + ", " + std::to_string(this->getEntity()->getChainTransform().getMatrix()[3][2]));
	//Error::printWarning("Camera worldpos: " + std::to_string(this->worldPosition.x) + ", " + std::to_string(this->worldPosition.y) + ", " + std::to_string(this->worldPosition.z));
}
