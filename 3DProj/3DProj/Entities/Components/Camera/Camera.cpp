#include "Camera.h"

#include "../../../Core/Display.h"
#include "..\..\Entity.h"
#include "../../../Error.h"
#include "../../../Utils/Utils.h"

#include <gtc\matrix_transform.hpp>

Camera::Camera(Display * display, glm::vec3 target, glm::vec3 relativePosition, float fov, float zNear, float zFar) //Used for normal "fps" camera
{
	this->display = display;
	this->relativePosition = relativePosition;
	this->target = target;
	this->orthoCam = false;
	this->active = false;
	this->zFar = zFar;
	this->zNear = zNear;
	this->fov = fov;
}

Camera::Camera(Display * display, float width, float height, glm::vec3 direction, glm::vec3 relativePosition, float zNear, float zFar) //Used for orthographic camera
{
	this->display = display;
	this->width = width;
	this->height = height;
	this->relativePosition = relativePosition;
	this->orthoCam = true;
	this->fov = 0.0f;
	this->zNear = zNear;
	this->zFar = zFar;
	this->active = false;
	this->f = glm::normalize(direction);
}

Camera::~Camera()
{
}

void Camera::init()
{
	this->isCClicked = 0;
	this->isCPressed = false;

	updateProj(this->fov, this->zNear, this->zFar);
	setWorldPosition();
	if (this->orthoCam)
		setDirection(this->f);
	else
		lookAt(this->target);

	rotate(this->yawPitchRoll.x, this->yawPitchRoll.y, this->yawPitchRoll.z);

	glfwSetCursorPos(this->display->getWindowPtr(), this->display->getWidth() / 2, this->display->getHeight() / 2);
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

		if (isCClicked) //If C is clicked, activate camera mouse movement
		{
			double xPos, yPos, sensitivity = 0.5f;

			// --------------------------------- Move Cursor ---------------------------------
			glfwGetCursorPos(this->display->getWindowPtr(), &xPos, &yPos);
			xPos -= this->display->getWidth() / 2;
			yPos -= this->display->getHeight() / 2;
			if (xPos != 0.0 || yPos != 0.0)
			{
				this->yawPitchRoll.x += (float)(xPos*dt*sensitivity);
				this->yawPitchRoll.y -= (float)(yPos*dt*sensitivity);

				if (yawPitchRoll.y > glm::radians(89.0f))
					yawPitchRoll.y = glm::radians(89.0f);
				if (yawPitchRoll.y < -glm::radians(89.0f))
					yawPitchRoll.y = -glm::radians(89.0f);
				
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

	this->yawPitchRoll.x = atan2(this->f.z, this->f.x);
	this->yawPitchRoll.y = atan2(this->f.y, -this->f.z);
	this->yawPitchRoll.z = atan2(this->u.y, this->f.x);
	
	//this->view = glm::lookAt(this->worldPosition, target, GLOBAL_UP_VECTOR);

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
	this->f.x = cos(yaw)*cos(pitch);
	this->f.y = sin(pitch);
	this->f.z = sin(yaw)*cos(pitch);
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
	this->f = glm::normalize(direction);
	this->r = glm::cross(f, GLOBAL_UP_VECTOR);
	this->u = glm::cross(r, f);

	this->yawPitchRoll.y = asin(direction.y);
	this->yawPitchRoll.x = atan2(direction.x, direction.z) - Tools::PI/2;
	this->yawPitchRoll.z = 0;


	updateView(this->f, this->r, this->u, this->worldPosition);
	this->getEntity()->getLocalTransform().setDirection(this->f);
}

bool Camera::isCamOrtho() const
{
	return this->orthoCam;
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

float Camera::getFarPlane() const
{
	return this->zFar;
}

float Camera::getNearPlane() const
{
	return this->zNear;
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
	glm::mat4 temp = this->getEntity()->getChainTransform().getMatrix()*this->localPositionMat;

	this->worldPosition = glm::vec3(temp[3][0], temp[3][1], temp[3][2]);
}
