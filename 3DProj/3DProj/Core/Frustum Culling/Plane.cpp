#include "Plane.h"



Plane::Plane()
{
}


Plane::~Plane()
{
}

void Plane::setPointAndNormal(glm::vec3 point, glm::vec3 normal)
{
	this->normal = normal;
	this->point = point;
}

float Plane::distance(const glm::vec3& point) const
{
	glm::vec3 v = point - this->point;
	float r = glm::dot(v, this->normal);

	return glm::length(point - this->point);
}
