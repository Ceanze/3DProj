#ifndef PHONG_SHADER_H
#define PHONG_SHADER_H

#include "ShaderProgram.h"

#include <glm.hpp>

class PhongShader : public ShaderProgram
{
public:
	PhongShader();
	virtual ~PhongShader();

private:
	void selfUpdateUniforms(Node* node);

	glm::mat4 proj;
	glm::mat4 view;
	glm::vec3 lightPos;
};

#endif
