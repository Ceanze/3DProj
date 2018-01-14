#include "Component.h"
#include "..\Entity.h"

Component::Component()
{

}

Component::~Component()
{
}

void Component::setEntity(Entity * ptr)
{
	this->entity = ptr;
}

Entity * Component::getEntity()
{
	return this->entity;
}
