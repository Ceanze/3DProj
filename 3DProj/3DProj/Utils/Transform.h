#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm.hpp>

class Transform
{
public:
	Transform();

	void setTranslation(const glm::vec3& translation);
	void setRotation(const glm::vec3& rotation);
	void setScale(const glm::vec3& scale);
	void setDirection(const glm::vec3& dir);
	void setLocalTranslation(const glm::vec3& translation);
	void setLocalRotation(const glm::vec3& rotation);
	void setLocalScale(const glm::vec3& scale);
	void setLocalDirection(const glm::vec3& dir);

	glm::vec3 getTranslation() const;
	glm::vec3 getRotation() const;
	glm::vec3 getScale() const;
	glm::vec3 getLocalTranslation() const;
	glm::vec3 getLocalRotation() const;
	glm::vec3 getLocalScale() const;

	void setWorldMatrix(const glm::mat4& worldMatrix);
	void setLocalMatrix(const glm::mat4& localMatrix);

	glm::mat4 getMVPMatrix() const;

	glm::mat4 getWorldMatrix() const;
	glm::mat4 getLocalMatrix() const;
	glm::mat4 getMatrix() const;

private:
	glm::mat4 worldMatrix;
	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 localMatrix;
	glm::vec3 localTranslation;
	glm::vec3 localRotation;
	glm::vec3 localScale;
};

#endif
