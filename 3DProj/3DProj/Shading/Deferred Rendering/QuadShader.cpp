#include "QuadShader.h"

#include "../../Error.h"

QuadShader::QuadShader() : ShaderProgram({ "Deferred Rendering/QuadDR.vs", GL_VERTEX_SHADER }, { "Deferred Rendering/QuadDR.fs", GL_FRAGMENT_SHADER })
{
	this->normalLoc = glGetUniformLocation(this->getID(), "diffuseTexture");
	if (normalLoc == -1)
		Error::printError("Could not find normalLoc");

	this->positionLoc = glGetUniformLocation(this->getID(), "specularTexture");
	if (positionLoc == -1)
		Error::printError("Could not find positionLoc");

	this->albedoLoc = glGetUniformLocation(this->getID(), "albedoTexture");
	if (albedoLoc == -1)
		Error::printError("Could not find albedoLoc");
}

QuadShader::~QuadShader()
{
}

void QuadShader::updateUniforms(GLuint* textures, unsigned nrOf)
{
	glUniform1i(this->positionLoc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glUniform1i(this->normalLoc, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);

	glUniform1i(this->albedoLoc, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textures[2]);


}

void QuadShader::selfUpdateUniforms(Node * entity)
{
}
