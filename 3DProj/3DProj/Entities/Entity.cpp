#include "Entity.h"
#include "Components\Component.h"

#include "../Shading/ShaderProgram.h"

#include <gtc\matrix_transform.hpp>
#include <string>

#include "../Error.h"

Entity::Entity(const glm::vec3 & position, const glm::vec3 & rotation, bool isDynamic)
{
	this->getWorldTransform().setTranslation(position);
	this->getWorldTransform().setRotation(rotation);
	this->isDynamic = isDynamic;
}

Entity::Entity(const glm::mat4 &matrix, bool isDynamic)
{
	this->getWorldTransform().setRotation({0.0f, 0.0f, 0.0f});
	this->isDynamic = isDynamic;
}

Entity::~Entity()
{
	for (size_t i = 0; i < this->components.size(); i++)
		delete this->components[i];
}

void Entity::addMesh(Mesh * mesh, ShaderProgram * shader)
{
	mesh->loadToGPU(shader->getID(), GL_STATIC_DRAW, true);
	this->meshes.push_back(mesh);
	if (this->shaderMap.find(shader) == this->shaderMap.end())
		this->shaders.push_back(shader);
	this->shaderMap[shader].push_back(this->meshes.size() - 1);
}

void Entity::addMeshes(const std::vector<Mesh*>& meshes, ShaderProgram * shader)
{
	for (Mesh* m : meshes)
		addMesh(m, shader);
}

void Entity::addComponent(Component* component)
{
	component->setEntity(this);
	this->components.push_back(component);
}

Component * Entity::getComponent(unsigned int index)
{
	return this->components[index];
}

std::vector<Mesh*> & Entity::getMeshes()
{
	return this->meshes;
}

void Entity::selfInit()
{
	for (size_t i = 0; i < this->components.size(); i++)
		this->components[i]->init();
}

void Entity::selfUpdate(float dt)
{
	for (size_t i = 0; i < this->components.size(); i++)
		this->components[i]->update(dt);
}

void Entity::selfRender(ShaderProgram* shadowShader)
{
	for (unsigned int i = 0; i < this->shaders.size(); i++)
	{
		std::vector<unsigned int> meshIds = this->shaderMap[this->shaders[i]];
		if (shadowShader == nullptr)
		{
			glUseProgram(this->shaders[i]->getID());
			this->shaders[i]->updateUniforms(this);
		}
		else
			shadowShader->updateUniforms(this);
		for (unsigned int j = 0; j < meshIds.size(); j++)
			this->meshes[meshIds[j]]->draw();
		if(shadowShader == nullptr)
			glUseProgram(0);
	}
}

void Entity::selfInput(Display * display)
{
	for (size_t i = 0; i < this->components.size(); i++)
		this->components[i]->input(display);
}
