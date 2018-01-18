#include "PhongLS.h"

#include "../../Error.h"

PhongLS::PhongLS() : ShaderProgram({ "Lightning Shaders/PhongLS.vs", GL_VERTEX_SHADER }, { "Lightning Shaders/PhongLS.fs", GL_FRAGMENT_SHADER })
{
	this->positionLoc = glGetUniformLocation(this->getID(), "positionTexture");
	if (positionLoc == -1)
		Error::printError("Could not find positionLoc");

	this->normalLoc = glGetUniformLocation(this->getID(), "normalTexture");
	if (normalLoc == -1)
		Error::printError("Could not find normalLoc");

	this->albedoLoc = glGetUniformLocation(this->getID(), "albedoTexture");
	if (albedoLoc == -1)
		Error::printError("Could not find albedoLoc");

	this->depthLoc = glGetUniformLocation(this->getID(), "depthTexture");
	if (depthLoc == -1)
		Error::printError("Could not find depthLoc");
}

PhongLS::~PhongLS()
{
}

void PhongLS::updateUniforms(FrameBuffer* buffer)
{
	glUniform1i(this->positionLoc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, buffer->getTexture(0));

	glUniform1i(this->normalLoc, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, buffer->getTexture(1));

	glUniform1i(this->albedoLoc, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, buffer->getTexture(2));

	glUniform1i(this->depthLoc, 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, buffer->getTexture(3));
}

void PhongLS::selfUpdateUniforms(Node * entity)
{
}
