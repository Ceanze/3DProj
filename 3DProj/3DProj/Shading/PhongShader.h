#ifndef PHONG_SHADEr_H
#define PHONG_SHADEr_H

#include "ShaderProgram.h"

#include <glm.hpp>

class PhongShader : public ShaderProgram
{
public:
	PhongShader();
	virtual ~PhongShader();

	void updateUniforms(Node* node);

private:
	glm::mat4 proj;
	glm::mat4 view;
	glm::vec3 lightPos;
};

#endif
