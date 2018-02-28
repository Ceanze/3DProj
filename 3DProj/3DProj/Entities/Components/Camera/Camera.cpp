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

	updateProj(this->fov, this->zNear, this->zFar);								//Sets the projection type, perspective or orthograpic
	setWorldPosition();															//Sets the cameras position in the world space with in consideration with the relative position
	if (this->orthoCam)
		setDirection(this->f);
	else
		lookAt(this->target);

	rotate(this->sphericalCoords.x, this->sphericalCoords.y); //Makes sure the rotation is right in the beginning

	glfwSetCursorPos(this->display->getWindowPtr(), this->display->getWidth() / 2, this->display->getHeight() / 2); //Sets the mouse pointer to the middle of the screen
}

void Camera::update(const float & dt)
{
	this->dt = dt;										//dt used for angle calc. in input
	setRelativePosition(this->relativePosition);		//Updates the position every frame
}

void Camera::input(Display * display)
{
	if (this->active) //If current camera is the active one
	{
		//----------Toogle for C----------------------
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
		//-------------------------------------------

		if (isCClicked) //If C is clicked, activate camera mouse movement
		{
			double xPos, yPos, sensitivity = 0.5f;

			// --------------------------------- Move Cursor ---------------------------------
			glfwGetCursorPos(this->display->getWindowPtr(), &xPos, &yPos); //Get the current mouse positions
			xPos -= this->display->getWidth() / 2;	//Calculate the distance they have travled from the center of the window
			yPos -= this->display->getHeight() / 2;
			if (xPos != 0.0 || yPos != 0.0) //If the mouse has moved
			{
				this->sphericalCoords.x += (float)(xPos*dt*sensitivity); //Calculate yaw with mouse x movement
				this->sphericalCoords.y -= (float)(yPos*dt*sensitivity); //Calculate pitch with mouse y movemnt

				if (sphericalCoords.y > glm::radians(89.0f)) //Lock the mouse for inverting on itself
					sphericalCoords.y = glm::radians(89.0f);
				if (sphericalCoords.y < -glm::radians(89.0f))
					sphericalCoords.y = -glm::radians(89.0f);
				
				rotate(this->sphericalCoords.x, this->sphericalCoords.y); //Update the rotation with the new yaw and pitch (roll isn't being used)
			}

			glfwSetCursorPos(this->display->getWindowPtr(), this->display->getWidth() / 2, this->display->getHeight() / 2); //Set the cursor to the middle of the screen again
			// -------------------------------------------------------------------------------

		}
	}
}

void Camera::lookAt(const glm::vec3 & target)
{
	this->f = glm::normalize(target - this->worldPosition);			//Calculate the camera forward direction
	this->r = glm::cross(f, GLOBAL_UP_VECTOR);						//Right vector
	this->u = glm::cross(r, f);										//The correct up vector
	
	this->sphericalCoords.x = atan2(this->f.z, this->f.x);			//Calculate the yaw pitch and roll with the new vectors
	this->sphericalCoords.y = atan2(this->f.y, -this->f.z);
	
	updateView(this->f, this->r, this->u, this->worldPosition);		//Update the view
	this->getEntity()->getLocalTransform().setDirection(this->f);	//Set the direction of the entity the camera is on the the same
}

void Camera::setRelativePosition(const glm::vec3 & relativePosition)
{
	this->relativePosition = relativePosition;						//Update relative position
	setWorldPosition();												//Set the new world position with the new relativePosition
	updateView(this->f, this->r, this->u, this->worldPosition);
}

void Camera::rotate(float phi, float theta)
{
	this->f.x = cos(phi)*cos(theta);								//calculate new f with the rotations in mind
	this->f.y = sin(theta);
	this->f.z = sin(phi)*cos(theta);
	this->r = glm::normalize(glm::cross(f, GLOBAL_UP_VECTOR));
	this->u = glm::cross(r, f);

	this->sphericalCoords.x = phi;									//If the function takes in yaw pitch and roll which is != yawPitchRoll.xyz, the update
	this->sphericalCoords.y = theta;

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

	this->sphericalCoords.x = atan2(this->f.z, this->f.x);
	this->sphericalCoords.y = atan2(this->f.y, -this->f.z);
	/*
	this->yawPitchRoll.y = asin(direction.y);
	this->yawPitchRoll.x = atan2(direction.x, direction.z) - Tools::PI/2;
	this->yawPitchRoll.z = 0;
	*/
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
		this->proj = glm::ortho(-this->width / 2, this->width / 2, -this->height / 2, this->height / 2, zNear, zFar); //Set orthograpic projection if that construct was used
	else
		this->proj = glm::perspective(fov, this->display->getRatio(), zNear, zFar);									  //Else set perspective
}

void Camera::updateView(const glm::vec3 & f, const glm::vec3 & r, const glm::vec3 & u, const glm::vec3 & pos)
{
	this->view = glm::mat4({ r.x, u.x, -f.x, 0.0f },						//Update the view
	{ r.y, u.y, -f.y, 0.0f },
	{ r.z, u.z, -f.z, 0.0f },
	{ -glm::dot(r, pos), -glm::dot(u, pos), glm::dot(f, pos), 1.0f });
}

void Camera::setLocalPositionMat()
{
	this->localPositionMat = glm::mat4(1, 0, 0, 0,
									   0, 1, 0, 0,
									   0, 0, 1, 0,
		this->relativePosition.x, this->relativePosition.y, this->relativePosition.z, 1);	//The translation put in the last column
}

void Camera::setWorldPosition()
{
	setLocalPositionMat();
	glm::mat4 temp = this->getEntity()->getChainTransform().getMatrix()*this->localPositionMat;		//Get the entity's position in the node tree, and add the relativeposition with respect to that

	this->worldPosition = glm::vec3(temp[3][0], temp[3][1], temp[3][2]);
}
