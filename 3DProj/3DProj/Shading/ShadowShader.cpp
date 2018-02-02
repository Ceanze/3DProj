#include "ShadowShader.h"

#include "../Error.h"

ShadowShader::ShadowShader() : ShaderProgram({ "ShadowShader.vs", GL_VERTEX_SHADER }, { "ShadowShader.fs", GL_FRAGMENT_SHADER })
{
	this->depthMat = glGetUniformLocation(this->getID(), "depthMat");
	if (this->depthMat == -1)
		Error::printError("Could not find depthMat!");
}

ShadowShader::~ShadowShader()
{
}

void ShadowShader::updateUniforms(GLuint * textures, unsigned nrOf)
{
	glUniform3fv(this->depthMat, 1, &(this->getCamera()->getPosition()[0]));
}

void ShadowShader::selfUpdateUniforms(Node * entity)
{
}
