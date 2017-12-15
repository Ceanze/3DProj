#ifndef MODEL_H
#define MODEL_H

#include <vector>

#include "Mesh.h"

class Model
{
public:
	Model();
	~Model();

private:
	std::vector<Mesh*> meshes;
};

#endif
