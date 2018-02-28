#include "Frustum.h"

#include "../../Entities/Components/Camera/Camera.h"
#include "../../Error.h"
#include "AABox.h"
#include "../Display.h"

void Frustum::calculateWidthAndHeight()
{
	float tang = tan(this->fov / 2);									//Calculate the tangent to get the height for the near and far plane (divide by two because it's half the frustum)
	this->nearHeight = this->camera->getNearPlane() * tang * 2.0f;		//Take the distance from the camera to the near plane * tang * 2 to get the full height
	this->nearWidth = this->nearHeight * this->ratio;					//Take the ratio * height to get the full width
	this->farHeight = this->camera->getFarPlane() * tang * 2.0f;
	this->farWidth = this->farHeight * this->ratio;
}

void Frustum::calculatePlanes()
{
	glm::vec3 point;
	glm::vec3 nearCenter = this->camPos + this->camera->getDirection() * zNear;							//Center of the near plane
	glm::vec3 farCenter = this->camPos + this->camera->getDirection() * zFar;							//Center of the far plane
	planes[NEAR_P].setPointAndNormal(nearCenter, this->camera->getDirection());							//The point and normal for the near and far planes are given
	planes[FAR_P].setPointAndNormal(farCenter, -this->camera->getDirection());

	point = nearCenter + this->camera->getUp() * (this->nearHeight / 2) - this->camera->getRight() * (this->nearWidth / 2);		//Top left corner of the near plane. This point is in both left and top plane
	planes[LEFT_P].setPointAndNormal(point, glm::normalize(cross(point - this->camPos, this->camera->getUp())));				//Calculate a normal which is pointing inwards to the frustum
	planes[TOP_P].setPointAndNormal(point, glm::normalize(cross(point - this->camPos, this->camera->getRight())));

	point = nearCenter - this->camera->getUp() * (this->nearHeight / 2) + this->camera->getRight() * (this->nearWidth / 2);		//Same as above, but with a point in the bottom right corner of the near plane
	planes[RIGHT_P].setPointAndNormal(point, glm::normalize(cross(point - this->camPos, -this->camera->getUp())));
	planes[BOTTOM_P].setPointAndNormal(point, glm::normalize(cross(point - this->camPos, -this->camera->getRight())));
}

Frustum::Frustum(Camera* camera, QuadTree* quadTree, float ratio)
{
	this->zNear = camera->getNearPlane();
	this->zFar = camera->getFarPlane();
	this->fov = camera->getFOV();
	this->ratio = ratio;
	this->camera = camera;
	this->quadTree = quadTree;
	calculateWidthAndHeight();
}


Frustum::~Frustum()
{
}

void Frustum::init()
{
	this->camPos = this->camera->getPosition();
	this->calculatePlanes();
}

void Frustum::update(glm::vec3 camPos)
{
	this->camPos = camPos;											//Update position
	this->calculatePlanes();										//Update the planes with the new position
	if (this->quadTree != nullptr)
		this->quadTree->statusFrustum(this->planes);				//Check if the quads are in the frustum
	else
		Error::printWarning("No 'QuadTree' attached to frustum!");
}

void Frustum::resize(const Display& display)
{
	this->ratio = display.getRatio();
	calculateWidthAndHeight();
}

float Frustum::getZNear() const
{
	return this->zNear;
}

float Frustum::getZFar() const
{
	return this->zFar;
}

float Frustum::getNearHeight() const
{
	return this->nearHeight;
}

float Frustum::getFarHeight() const
{
	return this->farHeight;
}

float Frustum::getNearWidth() const
{
	return this->nearWidth;
}

float Frustum::getFarWidth() const
{
	return this->farWidth;
}
