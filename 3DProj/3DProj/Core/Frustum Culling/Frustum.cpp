#include "Frustum.h"

#include "../../Entities/Components/Camera/Camera.h"
#include "../../Error.h"
#include "AABox.h"

void Frustum::calculateWidthAndHeight()
{
	float tang = tan(fov / 2);
	this->nearHeight = this->zNear * tang * 2.0f;
	this->nearWidth = this->nearHeight * this->ratio * 1.2f;
	this->farHeight = this->zFar * tang * 2.0f;
	this->farWidth = this->farHeight * this->ratio * 1.2f;
}

void Frustum::calculatePlanes()
{
	glm::vec3 point;
	glm::vec3 nearCenter = this->camPos + this->camera->getDirection() * zNear;
	glm::vec3 farCenter = this->camPos + this->camera->getDirection() * zFar;
	planes[NEAR_P].setPointAndNormal(nearCenter, this->camera->getDirection());
	planes[FAR_P].setPointAndNormal(farCenter, -this->camera->getDirection());

	point = nearCenter + this->camera->getUp() * (this->nearHeight / 2) - this->camera->getRight() * (this->nearWidth / 2);
	planes[LEFT_P].setPointAndNormal(point, glm::normalize(cross(point - camPos, this->camera->getUp())));
	planes[TOP_P].setPointAndNormal(point, glm::normalize(cross(point - camPos, this->camera->getRight())));

	point = nearCenter - this->camera->getUp() * (this->nearHeight / 2) + this->camera->getRight() * (this->nearWidth / 2);
	planes[RIGHT_P].setPointAndNormal(point, glm::normalize(cross(point - camPos, -this->camera->getUp())));
	planes[BOTTOM_P].setPointAndNormal(point, glm::normalize(cross(point - camPos, -this->camera->getRight())));
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
	this->camPos = camPos;
	this->calculatePlanes();
	if (this->quadTree != nullptr)
		this->quadTree->statusFrustum(this->planes);
	else
		Error::printWarning("No 'QuadTree' attached to frustum!");
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
