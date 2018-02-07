#include "AABox.h"




AABox::AABox(const std::vector<glm::vec3> points)
{
	this->points = points;
}

AABox::~AABox()
{
}

glm::vec3 AABox::getPoint(int index)
{
	return this->points[index];
}
