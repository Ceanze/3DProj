#include "GeometryShader.h"

#include "../../Error.h"

GeometryShader::GeometryShader() : ShaderProgram({ "Deferred Rendering/GeometryDR.vs", GL_VERTEX_SHADER }, { "Deferred Rendering/GeometryDR.fs", GL_FRAGMENT_SHADER }, { "Deferred Rendering/GeometryDR.gs", GL_GEOMETRY_SHADER })
{
}

GeometryShader::~GeometryShader()
{
}

void GeometryShader::updateUniforms(GLuint* textures, unsigned nrOf)
{
}

void GeometryShader::selfUpdateUniforms(Node * node)
{
	setUniformMatrix4fv("wm", 1, false, &(node->getChainTransform().getMatrix()*node->getLocalTransform().getMatrix())[0][0]);
	setUniform3fv("camPos", 1, &(getCamera()->getPosition())[0]);
	setUniform4fv("normal_ortho", 1, &(glm::vec4(getCamera()->getDirection(), (float)getCamera()->isCamOrtho()))[0]);
}
