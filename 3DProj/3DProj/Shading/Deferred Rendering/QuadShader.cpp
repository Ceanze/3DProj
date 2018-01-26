#include "QuadShader.h"

#include "../../Error.h"

QuadShader::QuadShader() : ShaderProgram({ "Deferred Rendering/QuadDR.vs", GL_VERTEX_SHADER }, { "Deferred Rendering/QuadDR.fs", GL_FRAGMENT_SHADER })
{
	this->textureLoc = glGetUniformLocation(this->getID(), "finalTexture");
	if (this->textureLoc == -1)
		Error::printError("Could not find finalTexture");
}

QuadShader::~QuadShader()
{
}

void QuadShader::updateUniforms(GLuint* textures, unsigned nrOf)
{
	glUniform1i(this->textureLoc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
}

void QuadShader::selfUpdateUniforms(Node * entity)
{
}
