#ifndef AABOX_H
#define AABOX_H

#include "../Config.h"
#include <vector>

class AABox
{
private:
	std::vector<glm::vec3> points;

public:
	AABox();
	~AABox();

	void addPoint(const glm::vec3& point);

	glm::vec3 getPoint(int index);
};

#endif