#include "Entity.h"

#include <gtc\matrix_transform.hpp>
#include <string>

#include "../Error.h"

Entity::Entity(const glm::vec3 & position, const glm::vec3 & direction, bool isDynamic)
{
	this->getWorldTransform().setLocalTranslation(position);
	this->getWorldTransform().setLocalDirection(direction);
	this->isDynamic = isDynamic;
}

Entity::Entity(const glm::mat4 &matrix, bool isDynamic)
{
	this->getWorldTransform().setDirection({0.0f, 0.0f, 1.0f});
	this->isDynamic = isDynamic;
}

Entity::~Entity()
{
}

void Entity::addMesh(Mesh * mesh, ShaderProgram * shader)
{
	mesh->loadToGPU(shader->getID(), GL_STATIC_DRAW, true);
	this->meshes.push_back(mesh);
	if (this->shaderMap.find(shader) == this->shaderMap.end())
	{
		this->shaders.push_back(shader);
		//this->shaderMap[shader] = std::vector<unsigned int>();
	}
	this->shaderMap[shader].push_back(this->meshes.size() - 1);
}

void Entity::selfUpdate(float dt)
{
	
}

void Entity::selfRender()
{
	for (unsigned int i = 0; i < this->shaders.size(); i++)
	{
		std::vector<unsigned int> meshIds = this->shaderMap[this->shaders[i]];
		glUseProgram(this->shaders[i]->getID());
		this->shaders[i]->updateUniforms(this);
		for (unsigned int j = 0; j < meshIds.size(); j++)
			this->meshes[meshIds[j]]->draw();
	}
}
