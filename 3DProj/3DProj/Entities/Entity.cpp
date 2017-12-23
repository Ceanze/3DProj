#include "Entity.h"

#include <gtc\matrix_transform.hpp>
#include <string>

#include "../Error.h"

Entity::Entity(const Model & model, const glm::vec3 & position, const glm::vec3 & direction, bool isDynamic)
{
	this->models.push_back(model);
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
	this->worldMatrix = glm::mat4({ -r.x, -r.y, -r.z, 0.0f }, { u.x, u.y, u.z, 0.0f }, { f.x, f.y, f.z, 0.0f }, { position.x, position.y, position.z, 1.0f });
	this->isDynamic = isDynamic;
	this->localMatrix = glm::mat4(1.0f);
}

Entity::Entity(const Model & model, const glm::mat4 &matrix, bool isDynamic)
{
	this->models.push_back(model);
	this->worldMatrix = matrix;
	this->isDynamic = isDynamic;
	this->localMatrix = glm::mat4(1.0f);
}

Entity::~Entity()
{
}

void Entity::draw(GLuint shaderProgramID)
{
	for (unsigned int i = 0; i < this->models.size(); i++)
	{
		updateMatrices(shaderProgramID, i);
		this->models[i].draw();
	}
}

void Entity::addModel(const Model & model)
{
	this->models.push_back(model);
}

void Entity::updateMatrices(GLuint shaderProgramID, unsigned int modelIndex)
{
	this->mwLoc = glGetUniformLocation(shaderProgramID, "mw");
	if(this->mwLoc == -1)
		Error::printError("Could not find 'mw' in shader program " + std::to_string(shaderProgramID) + "!");
	glUniformMatrix4fv(this->mwLoc, 1, GL_FALSE, &getTransformation(modelIndex)[0][0]);
}

Model & Entity::getModel(unsigned int index)
{
	return this->models.at(index);
}

std::vector<Model>& Entity::getModels()
{
	return this->models;
}

glm::mat4 Entity::getTransformation(unsigned int modelIndex) const
{
	return this->worldMatrix*this->localMatrix*this->models.at(modelIndex).getTransformation();
}

glm::mat4 Entity::getEntityMatrix() const
{
	return this->worldMatrix*this->localMatrix;
}

glm::mat4& Entity::getLocalMatrix() 
{
	return this->localMatrix;
}

glm::mat4& Entity::getWorldMatrix()
{
	return this->worldMatrix;
}

void Entity::setLocalMatrix(const glm::mat4 & mat)
{
	this->localMatrix = mat;
}

void Entity::setWorldMatrix(const glm::mat4 & mat)
{
	this->worldMatrix = mat;
}
