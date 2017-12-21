#ifndef MODEL_H
#define MODEL_H

#include <vector>

#include "Mesh.h"

class Model
{
public:
	Model(Mesh *mesh, const glm::vec3 &position, const glm::vec3 &direction, const float & scale);
	Model(Mesh *mesh, const glm::mat4 &matrix = glm::mat4(1.0f));
	~Model();

	void draw() const;

	std::vector<Mesh*> getMeshes() { return meshes; }

	void addMesh(Mesh *mesh);

	glm::mat4 getTransformation() const;
	glm::mat4 getLocalMatrix() const;
	glm::mat4 getWorldMatrix() const;

	void setLocalMatrix(const glm::mat4& mat);
	void setWorldMatrix(const glm::mat4& mat);

	void setScale(const float & scale);

private:
	std::vector<Mesh*> meshes;
	glm::mat4 localMatrix;
	glm::mat4 worldMatrix;
};

#endif
