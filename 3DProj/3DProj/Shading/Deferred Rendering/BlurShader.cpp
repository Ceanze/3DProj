#include "BlurShader.h"

#include "../../Error.h"

BlurShader::BlurShader() : ShaderProgram({ "Deferred Rendering/BlurShader.vs", GL_VERTEX_SHADER }, { "Deferred Rendering/BlurShader.fs", GL_FRAGMENT_SHADER })
{
	this->textureLoc = glGetUniformLocation(this->getID(), "finalTexture");
	if (this->textureLoc == -1)
		Error::printError("Could not find finalTexture");

	this->resolutionLoc = glGetUniformLocation(this->getID(), "textureResolution");
	if (this->resolutionLoc == -1)
		Error::printError("Could not find textureResolution");

	this->dirLoc = glGetUniformLocation(this->getID(), "dir");
	if (this->dirLoc == -1)
		Error::printError("Could not find dir");
}

BlurShader::~BlurShader()
{
}

void BlurShader::updateUniforms(GLuint * textures, unsigned nrOf)
{
	glUniform1i(this->textureLoc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
}

void BlurShader::sendTextureSize(const glm::vec2 & size)
{
	glUniform2fv(this->resolutionLoc, 1, &size[0]);
}

void BlurShader::sendDirection(const glm::vec2 & dir)
{
	glUniform2fv(this->dirLoc, 1, &dir[0]);
}

void BlurShader::selfUpdateUniforms(Node * entity)
{
}
