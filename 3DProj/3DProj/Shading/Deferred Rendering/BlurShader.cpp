#include "BlurShader.h"

#include "../../Error.h"

BlurShader::BlurShader() : ShaderProgram({ "Deferred Rendering/BlurShader.vs", GL_VERTEX_SHADER }, { "Deferred Rendering/BlurShader.fs", GL_FRAGMENT_SHADER })
{
}

BlurShader::~BlurShader()
{
}

void BlurShader::updateUniforms(GLuint * textures, unsigned nrOf)
{
	setTexture2D("finalTexture", 0, textures[0]);
}

void BlurShader::sendTextureSize(const glm::vec2 & size)
{
	setUniform2fv("textureResolution", 1, &size[0]);
}

void BlurShader::sendDirection(const glm::vec2 & dir)
{
	setUniform2fv("dir", 1, &dir[0]);
}

void BlurShader::selfUpdateUniforms(Node * entity)
{
}
