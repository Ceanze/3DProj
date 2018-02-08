#include "Frustum.h"

#include "../../Entities/Components/Camera/Camera.h"
#include "AABox.h"

void Frustum::calculateWidthAndHeight()
{
	float tang = tan(fov / 2);
	this->nearHeight = this->zNear * tang;
	this->nearWidth = this->nearHeight * this->ratio;
	this->farHeight = this->zFar * tang;
	this->farWidth = this->farHeight * this->ratio;
}

void Frustum::calculatePlanes()
{
	glm::vec3 point;
	glm::vec3 nearCenter = this->camPos - this->camera->getDirection() * zNear;
	glm::vec3 farCenter = this->camPos - this->camera->getDirection() * zFar;
	planes[NEAR].setPointAndNormal(nearCenter, this->camera->getDirection());
	planes[FAR].setPointAndNormal(farCenter, -this->camera->getDirection());

	point = nearCenter + (this->nearHeight / 2) - (this->nearWidth / 2);
	planes[TOP].setPointAndNormal(point, glm::normalize(cross(point - camPos, this->camera->getUp())));
	planes[LEFT].setPointAndNormal(point, glm::normalize(cross(point - camPos, -this->camera->getRight())));

	point = nearCenter - (this->nearHeight / 2) + (this->nearWidth / 2);
	planes[BOTTOM].setPointAndNormal(point, glm::normalize(cross(point - camPos, -this->camera->getUp())));
	planes[RIGHT].setPointAndNormal(point, glm::normalize(cross(point - camPos, this->camera->getRight())));
}

Frustum::Frustum(Camera* camera, float zNear, float zFar, float ratio)
{
	this->zNear = zNear;
	this->zFar = zFar;
	this->fov = camera->getFOV();
	this->ratio = ratio;
	
}


Frustum::~Frustum()
{
}

void Frustum::init(glm::vec3 camPos)
{
	this->camPos = camPos;
}

void Frustum::update(glm::vec3 camPos)
{
	this->camPos = camPos;
}

bool Frustum::checkBox(AABox &box)
{
	int in, out;
	bool result = false;

	for (int i = 0; i < 6; i++)
	{
		in = 0;
		out = 0;
		for (int j = 0; j < 8 && (in == 0 || out == 0); j++)
		{
			if (planes[i].distance(box.getPoint(j)) < 0)
				out++;
			else
				in++;
		}
		if (in == 0)
			return false;
		else
			result = true;
	}
}