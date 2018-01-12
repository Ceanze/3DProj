#include "Transform.h"

#include <gtc\matrix_transform.hpp>

Transform::Transform()
{
	this->translation = { 0.0f, 0.0f, 0.0f };
	this->rotation = { 0.0f, 0.0f, 0.0f };
	this->scale = { 1.0f, 1.0f, 1.0f };
	
	this->localTranslation = { 0.0f, 0.0f, 0.0f };
	this->localRotation = { 0.0f, 0.0f, 0.0f };
	this->localScale = { 1.0f, 1.0f, 1.0f };

	this->worldMatrix = glm::mat4(1.0f);
	this->localMatrix = glm::mat4(1.0f);
}

void Transform::setTranslation(const glm::vec3 & translation)
{
	this->translation = translation;
	this->worldMatrix[3][0] = translation.x;
	this->worldMatrix[3][1] = translation.y;
	this->worldMatrix[3][2] = translation.z;
}

void Transform::setRotation(const glm::vec3 & rotation)
{
	this->rotation = rotation;
	this->worldMatrix = glm::mat4(1.0f);
	setTranslation(this->translation);
	setScale(this->scale);
	// This might be worng order. TODO: Make this myself instead of using glm::rotate.
	glm::rotate(this->worldMatrix, rotation.x, {0.0f, 1.0f, 0.0f});
	glm::rotate(this->worldMatrix, rotation.y, {0.0f, 1.0f, 0.0f});
	glm::rotate(this->worldMatrix, rotation.z, {0.0f, 1.0f, 0.0f});
}

void Transform::setScale(const glm::vec3 & scale)
{
	this->scale = scale;
	this->worldMatrix[0][0] = scale.x;
	this->worldMatrix[1][1] = scale.y;
	this->worldMatrix[2][2] = scale.z;
}

void Transform::setLocalTranslation(const glm::vec3 & translation)
{
	this->localTranslation = translation;
	this->localMatrix[3][0] = translation.x;
	this->localMatrix[3][1] = translation.y;
	this->localMatrix[3][2] = translation.z;
}

void Transform::setLocalRotation(const glm::vec3 & rotation)
{
	this->localRotation = rotation;
	this->localMatrix = glm::mat4(1.0f);
	setLocalTranslation(this->localTranslation);
	setLocalScale(this->localScale);
	// This might be worng order. TODO: Make this myself instead of using glm::rotate.
	glm::rotate(this->localMatrix, rotation.x, {0.0f, 1.0f, 0.0f});
	glm::rotate(this->localMatrix, rotation.y, { 0.0f, 1.0f, 0.0f });
	glm::rotate(this->localMatrix, rotation.z, { 0.0f, 1.0f, 0.0f });
}

void Transform::setLocalScale(const glm::vec3 & scale)
{
	this->localScale = scale;
	this->localMatrix[0][0] = scale.x;
	this->localMatrix[1][1] = scale.y;
	this->localMatrix[2][2] = scale.z;
}

glm::vec3 Transform::getTranslation() const
{
	return this->translation;
}

glm::vec3 Transform::getRotation() const
{
	return this->rotation;
}

glm::vec3 Transform::getScale() const
{
	return this->scale;
}

glm::vec3 Transform::getLocalTranslation() const
{
	return this->localTranslation;
}

glm::vec3 Transform::getLocalRotation() const
{
	return this->localRotation;
}

glm::vec3 Transform::getLocalScale() const
{
	return this->localScale;
}

void Transform::setWorldMatrix(const glm::mat4 & worldMatrix)
{
	this->worldMatrix = worldMatrix;
}

void Transform::setLocalMatrix(const glm::mat4 & localMatrix)
{
	this->localMatrix = localMatrix;
}

glm::mat4 Transform::getMVPMatrix() const
{
	// TODO: Implement this!
	return glm::mat4(1.0f);
}

glm::mat4 Transform::getWorldMatrix() const
{
	return this->worldMatrix;
}

glm::mat4 Transform::getLocalMatrix() const
{
	return this->localMatrix;
}

glm::mat4 Transform::getMatrix() const
{
	return getWorldMatrix()*getLocalMatrix();
}
