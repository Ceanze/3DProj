#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <glm.hpp>

#include "Models\Model.h"

class Entity
{
public:
	Entity();
	~Entity();

	glm::mat4& getTransformation();

private:
	std::vector<Model> models;
	bool isDynamic;

	glm::mat4 worldMatrix;
};

#endif
