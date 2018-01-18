#include "QuadShader.h"

QuadShader::QuadShader() : ShaderProgram({ "Deferred Rendering/QuadDR.vs", GL_VERTEX_SHADER }, { "Deferred Rendering/QuadDR.fs", GL_FRAGMENT_SHADER })
{
}

QuadShader::~QuadShader()
{
}

void QuadShader::selfUpdateUniforms(Node * node)
{
}
