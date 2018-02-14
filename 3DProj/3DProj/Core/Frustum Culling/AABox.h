#ifndef AABOX_H
#define AABOX_H

#include "glm.hpp"
#include <vector>

class AABox
{
private:
	std::vector<glm::vec3> points;
	glm::vec3 sum;
public:
	AABox();
	~AABox();

	void addPoint(const glm::vec3& point);

	glm::vec3 getPoint(int index);
	glm::vec3 getCenterPoint();
};

#endif