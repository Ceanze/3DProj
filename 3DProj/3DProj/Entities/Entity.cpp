#include "Entity.h"

Entity::Entity()
{
	this->isDynamic = false;
}

Entity::~Entity()
{
}

glm::mat4& Entity::getTransformation()
{
	return this->worldMatrix*this->models.at(0).getTransformation();
}
