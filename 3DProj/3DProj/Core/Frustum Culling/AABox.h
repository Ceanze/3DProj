#ifndef AABOX_H
#define AABOX_H

#include "../Config.h"
#include <vector>

class AABox
{
private:
	std::vector<glm::vec3> points;

public:
	AABox(const std::vector<glm::vec3> points);
	~AABox();

	glm::vec3 getPoint(int index);
};

#endif