#include "Transform.h"

#include <gtc\matrix_transform.hpp>
#include <gtc\quaternion.hpp>
#include <gtx\quaternion.hpp>

Transform::Transform()
{
	this->translation = { 0.0f, 0.0f, 0.0f };
	this->rotation = { 0.0f, 0.0f, 0.0f };
	this->scale = { 1.0f, 1.0f, 1.0f };

	this->matrix = glm::mat4(1.0f);
}

void Transform::setTranslation(const glm::vec3 & translation)
{
	this->translation = translation;
	this->matrix[3][0] = translation.x;
	this->matrix[3][1] = translation.y;
	this->matrix[3][2] = translation.z;
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

	this->matrix = rotZ*rotY*rotX*scaleMat;
	setTranslation(this->translation);
}

void Transform::setScale(const glm::vec3 & scale)
{
	this->scale = scale;
	this->matrix[0][0] = scale.x;
	this->matrix[1][1] = scale.y;
	this->matrix[2][2] = scale.z;
}

void Transform::setDirection(const glm::vec3 & dir)
{
	const glm::vec3 f = dir;
	glm::vec3 r;

	if (glm::abs(glm::dot(f,glm::vec3(0.0f, 1.0f, 0.0f))) > 0.99f)
	{
		r = glm::normalize(glm::cross(f, glm::vec3(1.0f, 0.0f, 0.0f)));
	}
	else
	{
		r = glm::normalize(glm::cross(f, glm::vec3(0.0f, 1.0f, 0.0f)));
	}

	const glm::vec3 u = glm::cross(r, f);
	const glm::mat4 scaleMat = glm::mat4{ { this->scale.x, 0.0f, 0.0f, 0.0f },{ 0.0f, this->scale.y, 0.0f, 0.0f },{ 0.0f, 0.0f, this->scale.z, 0.0f },{ 0.0f, 0.0f, 0.0f, 1.0f } };

	this->matrix = glm::mat4({ -r.x, -r.y, -r.z, 0.0f }, { u.x, u.y, u.z, 0.0f }, { f.x, f.y, f.z, 0.0f }, { this->translation.x, this->translation.y, this->translation.z, 1.0f })*scaleMat;
}

glm::vec3 Transform::getTranslation() const
{
	return glm::vec3(this->matrix[3][0],
					 this->matrix[3][1],
					 this->matrix[3][2]);
}

glm::vec3 Transform::getRotation() const
{
	return this->rotation;
}

glm::vec3 Transform::getScale() const
{
	return this->scale;
}

void Transform::setMatrix(const glm::mat4 & matrix)
{
	this->matrix = matrix;
}

glm::mat4 Transform::getMatrix() const
{
	return this->matrix;
}
