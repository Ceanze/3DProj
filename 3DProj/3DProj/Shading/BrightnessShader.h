#ifndef BRIGHTNESS_SHADER_H
#define BRIGHTNESS_SHADER_H

#include "ShaderProgram.h"

class BrightnessShader : public ShaderProgram
{
public:
	BrightnessShader();
	virtual ~BrightnessShader();

	void updateUniforms(GLuint* textures, unsigned nrOf);

private:
	void selfUpdateUniforms(Node* entity);
};

#endif
