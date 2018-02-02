#include "CombineShader.h"

#include "../../Error.h"

CombineShader::CombineShader() : ShaderProgram({ "Deferred Rendering/CombineShader.vs", GL_VERTEX_SHADER }, { "Deferred Rendering/CombineShader.fs", GL_FRAGMENT_SHADER })
{
	this->diffuseLoc = glGetUniformLocation(this->getID(), "diffuseTexture");
	if (this->diffuseLoc == -1)
		Error::printError("Could not find diffuseTexture");

	this->specularLoc = glGetUniformLocation(this->getID(), "specularTexture");
	if (this->specularLoc == -1)
		Error::printError("Could not find specularTexture");

	this->albedoLoc = glGetUniformLocation(this->getID(), "albedoTexture");
	if (this->albedoLoc == -1)
		Error::printError("Could not find albedoTexture");
}

CombineShader::~CombineShader()
{
}

void CombineShader::updateUniforms(GLuint * textures, unsigned nrOf)
{
	glUniform1i(this->diffuseLoc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glUniform1i(this->specularLoc, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);

	glUniform1i(this->albedoLoc, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textures[2]);
}

void CombineShader::selfUpdateUniforms(Node * entity)
{
}