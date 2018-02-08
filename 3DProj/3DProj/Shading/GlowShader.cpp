#include "GlowShader.h"

#include "../Error.h"

GlowShader::GlowShader() : ShaderProgram({ "GlowShader.vs", GL_VERTEX_SHADER }, { "GlowShader.fs", GL_FRAGMENT_SHADER })
{
}

GlowShader::~GlowShader()
{
}

void GlowShader::updateUniforms(GLuint * textures, unsigned nrOf)
{
	setTexture2D("finalTexture", 0, textures[0]);
	setTexture2D("glowTexture", 1, textures[1]);
}

void GlowShader::selfUpdateUniforms(Node * entity)
{
}
