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

	this->camLoc = glGetUniformLocation(this->getID(), "camPos");
	if (camLoc != -1)
		Error::printError("Could not find camPos in PhongLS");
}

PhongLS::~PhongLS()
{
}

void PhongLS::updateUniforms(GLuint* textures, unsigned nrOf)
{
	glUniform3fv(this->camLoc, 1, &(this->getCamera()->getPosition()[0]));

	glUniform1i(this->positionLoc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glUniform1i(this->normalLoc, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
}

void PhongLS::selfUpdateUniforms(Node * entity)
{
}
