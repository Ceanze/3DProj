#include "AABox.h"




AABox::AABox()
{

}

AABox::~AABox()
{
}

void AABox::addPoint(const glm::vec3 & point)
{
	this->points.push_back(point);
}

glm::vec3 AABox::getPoint(int index)
{
	return this->points[index];
}
