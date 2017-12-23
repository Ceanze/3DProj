#include "Model.h"

#include <gtc\matrix_transform.hpp>

Model::Model(Mesh * mesh, const glm::vec3 & position, const glm::vec3 & direction, const float & scale)
{
	this->meshes.push_back(mesh);
	
	const glm::vec3 f = direction;
	glm::vec3 r;

	if (f == glm::vec3(0.0f, 1.0f, 0.0f))
	{
		r = glm::normalize(glm::cross(f, glm::vec3(1.0f, 0.0f, 0.0f)));
	}
	else
	{
		r = glm::normalize(glm::cross(f, glm::vec3(0.0f, 1.0f, 0.0f)));
	}

	const glm::vec3 u = glm::cross(r, f);
	this->worldMatrix = glm::mat4({-r.x, -r.y, -r.z, 0.0f}, {u.x, u.y, u.z, 0.0f}, {f.x, f.y, f.z, 0.0f}, {position.x, position.y, position.z, 1.0f});
	this->localMatrix = glm::mat4(1.0f);

	setScale(scale);
}

Model::Model(Mesh * mesh, const glm::mat4 & matrix)
{
	this->meshes.push_back(mesh);
	this->worldMatrix = matrix;
	this->localMatrix = glm::mat4(1.0f);
}

Model::~Model()
{
}

void Model::draw() const
{
	for (unsigned int i = 0; i < this->meshes.size(); i++)
		this->meshes[i]->draw();
}

void Model::addMesh(Mesh * mesh)
{
	this->meshes.push_back(mesh);
}

std::vector<Mesh*>& Model::getMesh()
{
	return this->meshes;
}

glm::mat4 Model::getTransformation() const
{
	return this->worldMatrix*this->localMatrix;
}

glm::mat4& Model::getLocalMatrix()
{
	return this->localMatrix;
}

glm::mat4& Model::getWorldMatrix()
{
	return this->worldMatrix;
}

void Model::setLocalMatrix(const glm::mat4 & mat)
{
	this->localMatrix = mat;
}

void Model::setWorldMatrix(const glm::mat4 & mat)
{
	this->worldMatrix = mat;
}

void Model::setScale(const float & scale)
{
	this->worldMatrix = glm::scale(this->worldMatrix, glm::vec3(scale, scale, scale));
}
