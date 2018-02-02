#include "BrightnessShader.h"

#include "../Error.h"

BrightnessShader::BrightnessShader() : ShaderProgram({ "Deferred Rendering/BrightnessShader.vs", GL_VERTEX_SHADER }, { "Deferred Rendering/BrightnessShader.fs", GL_FRAGMENT_SHADER })
{
	this->textureLoc = glGetUniformLocation(this->getID(), "finalTexture");
	if (this->textureLoc == -1)
		Error::printError("Could not find finalTexture");
}

BrightnessShader::~BrightnessShader()
{
}

void BrightnessShader::updateUniforms(GLuint * textures, unsigned nrOf)
{
	glUniform1i(this->textureLoc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
}

void BrightnessShader::selfUpdateUniforms(Node * entity)
{
}
