#ifndef QUAD_SHADER_H
#define QUAD_SHADER_H

#include "../ShaderProgram.h"

#include <glm.hpp>

class QuadShader : public ShaderProgram
{
public:
	QuadShader();
	virtual ~QuadShader();

private:
	void selfUpdateUniforms(Node* node);
};

#endif
