#ifndef QUAD_SHADER_H
#define QUAD_SHADER_H

#include "../ShaderProgram.h"


class QuadShader : public ShaderProgram
{
public:
	QuadShader();
	virtual ~QuadShader();

	void updateUniforms(GLuint* textures, unsigned nrOf);
private:
	void selfUpdateUniforms(Node* entity);

	GLint diffuseLoc, specularLoc, albedoLoc;
};

#endif
