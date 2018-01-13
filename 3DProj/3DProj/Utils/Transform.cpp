#include "Transform.h"

#include <gtc\matrix_transform.hpp>
#include <gtc\quaternion.hpp>
#include <gtx\quaternion.hpp>

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

	const float cosX = glm::cos(rotation.x);
	const float sinX = glm::sin(rotation.x);
	const glm::mat4 rotX = glm::mat4{ {1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, cosX, sinX, 0.0f}, {0.0f, -sinX, cosX, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f} };

	const float cosY = glm::cos(rotation.y);
	const float sinY = glm::sin(rotation.y);
	const glm::mat4 rotY = glm::mat4{ { cosY, 0.0f, -sinY, 0.0f },{ 0.0f, 1.0f, 0.0f, 0.0f },{ sinY, 0.0f, cosY, 0.0f },{ 0.0f, 0.0f, 0.0f, 1.0f } };

	const float cosZ = glm::cos(rotation.z);
	const float sinZ = glm::sin(rotation.z);
	const glm::mat4 rotZ = glm::mat4{ { cosZ, sinZ, 0.0f, 0.0f },{ -sinZ, cosZ, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f, 0.0f },{ 0.0f, 0.0f, 0.0f, 1.0f } };

	const glm::mat4 scaleMat = glm::mat4{ { this->scale.x, 0.0f, 0.0f, 0.0f },{ 0.0f, this->scale.y, 0.0f, 0.0f },{ 0.0f, 0.0f, this->scale.z, 0.0f },{ 0.0f, 0.0f, 0.0f, 1.0f } };

	this->worldMatrix = rotZ*rotY*rotX*scaleMat;
	setTranslation(this->translation);
}

void Transform::setScale(const glm::vec3 & scale)
{
	this->scale = scale;
	this->worldMatrix[0][0] = scale.x;
	this->worldMatrix[1][1] = scale.y;
	this->worldMatrix[2][2] = scale.z;
}

void Transform::setDirection(const glm::vec3 & dir)
{
	const glm::vec3 f = dir;
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
	const glm::mat4 scaleMat = glm::mat4{ { this->scale.x, 0.0f, 0.0f, 0.0f },{ 0.0f, this->scale.y, 0.0f, 0.0f },{ 0.0f, 0.0f, this->scale.z, 0.0f },{ 0.0f, 0.0f, 0.0f, 1.0f } };

	this->worldMatrix = glm::mat4({ -r.x, -r.y, -r.z, 0.0f }, { u.x, u.y, u.z, 0.0f }, { f.x, f.y, f.z, 0.0f }, { this->translation.x, this->translation.y, this->translation.z, 1.0f })*scaleMat;
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

	const float cosX = glm::cos(rotation.x);
	const float sinX = glm::sin(rotation.x);
	const glm::mat4 rotX = glm::mat4{ { 1.0f, 0.0f, 0.0f, 0.0f },{ 0.0f, cosX, sinX, 0.0f },{ 0.0f, -sinX, cosX, 0.0f },{ 0.0f, 0.0f, 0.0f, 1.0f } };

	const float cosY = glm::cos(rotation.y);
	const float sinY = glm::sin(rotation.y);
	const glm::mat4 rotY = glm::mat4{ { cosY, 0.0f, -sinY, 0.0f },{ 0.0f, 1.0f, 0.0f, 0.0f },{ sinY, 0.0f, cosY, 0.0f },{ 0.0f, 0.0f, 0.0f, 1.0f } };

	const float cosZ = glm::cos(rotation.z);
	const float sinZ = glm::sin(rotation.z);
	const glm::mat4 rotZ = glm::mat4{ { cosZ, sinZ, 0.0f, 0.0f },{ -sinZ, cosZ, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f, 0.0f },{ 0.0f, 0.0f, 0.0f, 1.0f } };

	const glm::mat4 scaleMat = glm::mat4{ { this->localScale.x, 0.0f, 0.0f, 0.0f },{ 0.0f, this->localScale.y, 0.0f, 0.0f },{ 0.0f, 0.0f, this->localScale.z, 0.0f },{ 0.0f, 0.0f, 0.0f, 1.0f } };

	this->localMatrix = rotZ*rotY*rotX*scaleMat;
	setLocalTranslation(this->localTranslation);
}

void Transform::setLocalScale(const glm::vec3 & scale)
{
	this->localScale = scale;
	this->localMatrix[0][0] = scale.x;
	this->localMatrix[1][1] = scale.y;
	this->localMatrix[2][2] = scale.z;
}

void Transform::setLocalDirection(const glm::vec3 & dir)
{
	const glm::vec3 f = dir;
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
	const glm::mat4 scaleMat = glm::mat4{ { this->localScale.x, 0.0f, 0.0f, 0.0f },{ 0.0f, this->localScale.y, 0.0f, 0.0f },{ 0.0f, 0.0f, this->localScale.z, 0.0f },{ 0.0f, 0.0f, 0.0f, 1.0f } };

	this->localMatrix = glm::mat4({ -r.x, -r.y, -r.z, 0.0f }, { u.x, u.y, u.z, 0.0f }, { f.x, f.y, f.z, 0.0f }, { this->localTranslation.x, this->localTranslation.y, this->localTranslation.z, 1.0f })*scaleMat;
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
