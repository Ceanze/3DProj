#include "Model.h"

Model::Model()
{
}

Model::~Model()
{
	unsigned int numMeshes = this->meshes.size();
	for (unsigned int i = 0; i < numMeshes; i++)
		delete this->meshes[i];
}

glm::mat4& Model::getTransformation()
{
	return this->modelMatrix;
}
