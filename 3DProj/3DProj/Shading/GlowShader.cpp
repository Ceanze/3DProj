#include "GlowShader.h"

#include "../Error.h"

GlowShader::GlowShader() : ShaderProgram({ "GlowShader.vs", GL_VERTEX_SHADER }, { "GlowShader.fs", GL_FRAGMENT_SHADER })
{
	this->textureLoc = glGetUniformLocation(this->getID(), "finalTexture");
	if (this->textureLoc == -1)
		Error::printError("Could not find finalTexture");

	this->glowLoc = glGetUniformLocation(this->getID(), "glowTexture");
	if (this->textureLoc == -1)
		Error::printError("Could not find glowTexture");
}

GlowShader::~GlowShader()
{
}

void GlowShader::updateUniforms(GLuint * textures, unsigned nrOf)
{
	glUniform1i(this->textureLoc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glUniform1i(this->glowLoc, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
}

void GlowShader::selfUpdateUniforms(Node * entity)
{
}
