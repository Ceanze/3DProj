#ifndef ENTITY_H
#define ENTITY_H

#include "Models\Model.h"
#include "../Loaders/ObjLoader.h"

class Entity
{
public:
	Entity();
	~Entity();

private:
	Model model;
	bool isDynamic;

	static ObjLoader loader;
};

#endif
