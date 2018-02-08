#ifndef GLOW_SHADER_H
#define GLOW_SHADER_H

#include "ShaderProgram.h"

class GlowShader : public ShaderProgram
{
public:
	GlowShader();
	virtual ~GlowShader();

	void updateUniforms(GLuint* textures, unsigned nrOf);

private:
	void selfUpdateUniforms(Node* entity);
};

#endif
