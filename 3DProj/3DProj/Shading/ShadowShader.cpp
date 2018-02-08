#include "ShadowShader.h"

#include "../Error.h"

ShadowShader::ShadowShader() : ShaderProgram({ "ShadowShader.vs", GL_VERTEX_SHADER }, { "ShadowShader.fs", GL_FRAGMENT_SHADER })
{
}

ShadowShader::~ShadowShader()
{
}

void ShadowShader::updateUniforms(GLuint * textures, unsigned nrOf)
{
}

void ShadowShader::selfUpdateUniforms(Node * entity)
{
	setUniformMatrix4fv("camera", 1, GL_FALSE, &(this->getCamera()->getVP())[0][0]);
	setUniformMatrix4fv("wm", 1, GL_FALSE, &(entity->getChainTransform().getMatrix()*entity->getLocalTransform().getMatrix())[0][0]);
}
