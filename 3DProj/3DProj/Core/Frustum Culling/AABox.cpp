#include "AABox.h"




AABox::AABox()
{
	this->sum = glm::vec3(0.0f);
}

AABox::~AABox()
{
}

void AABox::addPoint(const glm::vec3 & point)
{
	this->sum += point;
	this->points.push_back(point);
}

glm::vec3 AABox::getPoint(int index)
{
	return this->points[index];
}

glm::vec3 AABox::getCenterPoint()
{
	return this->sum/(float)this->points.size();
}
