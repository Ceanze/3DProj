#include "BrightnessShader.h"

#include "../Error.h"

BrightnessShader::BrightnessShader() : ShaderProgram({ "Deferred Rendering/BrightnessShader.vs", GL_VERTEX_SHADER }, { "Deferred Rendering/BrightnessShader.fs", GL_FRAGMENT_SHADER })
{
}

BrightnessShader::~BrightnessShader()
{
}

void BrightnessShader::updateUniforms(GLuint * textures, unsigned nrOf)
{
	setTexture2D("finalTexture", 0, textures[0]);
}

void BrightnessShader::selfUpdateUniforms(Node * entity)
{
}
