#include "PhongLS.h"

#include "../../Error.h"

PhongLS::PhongLS() : ShaderProgram({ "Lightning Shaders/PhongLS.vs", GL_VERTEX_SHADER }, { "Lightning Shaders/PhongLS.fs", GL_FRAGMENT_SHADER })
{

}


PhongLS::~PhongLS()
{
}

void PhongLS::updateUniforms(FrameBuffer* buffer)
{
	GLint positionLoc = glGetUniformLocation(this->getID(), "positionTexture");
	if (positionLoc == -1)
		Error::printError("Could not find positionLoc");

	GLint normalLoc = glGetUniformLocation(this->getID(), "normalTexture");
	if (positionLoc == -1)
		Error::printError("Could not find normalLoc");

	GLint albedoLoc = glGetUniformLocation(this->getID(), "albedoTexture");
	if (positionLoc == -1)
		Error::printError("Could not find albedoLoc");

	GLint depthLoc = glGetUniformLocation(this->getID(), "depthTexture");
	if (positionLoc == -1)
		Error::printError("Could not find depthLoc");

	glUniform1i(positionLoc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, buffer->getTexture(0));

	glUniform1i(normalLoc, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, buffer->getTexture(1));

	glUniform1i(albedoLoc, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, buffer->getTexture(2));

	glUniform1i(depthLoc, 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, buffer->getTexture(3));
}

void PhongLS::selfUpdateUniforms(Node * entity)
{
}
