#include "CombineShader.h"

#include "../../Error.h"

CombineShader::CombineShader() : ShaderProgram({ "Deferred Rendering/CombineShader.vs", GL_VERTEX_SHADER }, { "Deferred Rendering/CombineShader.fs", GL_FRAGMENT_SHADER })
{
}

CombineShader::~CombineShader()
{
}

void CombineShader::updateUniforms(GLuint * textures, unsigned nrOf)
{
	setTexture2D("diffuseTexture", 0, textures[0]);
	setTexture2D("specularTexture", 1, textures[1]);
	setTexture2D("albedoTexture", 2, textures[2]);
}

void CombineShader::selfUpdateUniforms(Node * entity)
{
}
