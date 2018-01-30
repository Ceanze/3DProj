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

	glm::vec3 getTranslation() const;
	glm::vec3 getRotation() const;
	glm::vec3 getScale() const;
	glm::vec3 getDirection() const;

	void setMatrix(const glm::mat4& matrix);

	glm::mat4 getMatrix() const;

private:
	glm::vec3 direction;
	glm::mat4 matrix;
	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scale;
};

#endif
