#ifndef PLANE_H
#define PLANE_H

#include "glm.hpp"

class Plane
{
private:
	glm::vec3 normal, point;

public:
	Plane();
	~Plane();

	void setPointAndNormal(glm::vec3 point, glm::vec3 normal);
	float distance(const glm::vec3& point) const;
};

#endif