#include "ShadowShader.h"

#include "../Error.h"

ShadowShader::ShadowShader() : ShaderProgram({ "ShadowShader.vs", GL_VERTEX_SHADER }, { "ShadowShader.fs", GL_FRAGMENT_SHADER })
{
	this->wmLoc = glGetUniformLocation(this->getID(), "wm");
	if (this->wmLoc == -1)
		Error::printError("Could not find wm!");

	this->depthCameraLoc = glGetUniformLocation(this->getID(), "camera");
	if (this->depthCameraLoc == -1)
		Error::printError("Could not find camera!");
}

ShadowShader::~ShadowShader()
{
}

void ShadowShader::updateUniforms(GLuint * textures, unsigned nrOf)
{
}

void ShadowShader::selfUpdateUniforms(Node * entity)
{
	glUniformMatrix4fv(this->depthCameraLoc, 1, GL_FALSE, &(this->getCamera()->getVP())[0][0]);
	glUniformMatrix4fv(this->wmLoc, 1, GL_FALSE, &(entity->getChainTransform().getMatrix()*entity->getLocalTransform().getMatrix())[0][0]);
}
