#ifndef TEST_SHADER_H
#define TEST_SHADER_H

#include "ShaderProgram.h"

#include <glm.hpp>

class TestShader : public ShaderProgram
{
public:
	TestShader();
	virtual ~TestShader();

	void updateUniforms(Node* node);

	float& getTime();

private:
	glm::mat4 proj;
	glm::mat4 view;
	glm::vec3 lightPos;

	float time;
};

#endif
