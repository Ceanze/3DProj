#ifndef SHADOW_SHADER_H
#define SHADOW_SHADER_H

#include "ShaderProgram.h"

class ShadowShader : public ShaderProgram
{
public:
	ShadowShader();
	virtual ~ShadowShader();

	void updateUniforms(GLuint* textures, unsigned nrOf);

private:
	void selfUpdateUniforms(Node* entity);

	GLuint depthCameraLoc, wmLoc;
};

#endif
