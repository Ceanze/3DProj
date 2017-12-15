#ifndef MODEL_H
#define MODEL_H

#include <vector>

#include "Mesh.h"

class Model
{
public:
	Model();
	~Model();

	glm::mat4& getTransformation();

private:
	std::vector<Mesh*> meshes;
	glm::mat4 modelMatrix;
};

#endif
