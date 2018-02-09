#include "QuadShader.h"

#include "../../Error.h"

QuadShader::QuadShader() : ShaderProgram({ "Deferred Rendering/QuadDR.vs", GL_VERTEX_SHADER }, { "Deferred Rendering/QuadDR.fs", GL_FRAGMENT_SHADER })
{
}

QuadShader::~QuadShader()
{
}

void QuadShader::updateUniforms(GLuint* textures, unsigned nrOf)
{
	setTexture2D("finalTexture", 0, textures[0]);
}

void QuadShader::selfUpdateUniforms(Node * entity)
{
}
