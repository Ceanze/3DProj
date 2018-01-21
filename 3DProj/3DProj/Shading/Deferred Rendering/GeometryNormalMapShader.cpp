#include "GeometryNormalMapShader.h"

GeometryNormalMapShader::GeometryNormalMapShader() : ShaderProgram({ "Deferred Rendering/GeometryDRNormalMap.vs", GL_VERTEX_SHADER }, { "Deferred Rendering/GeometryDRNormalMap.fs", GL_FRAGMENT_SHADER })
{
}

GeometryNormalMapShader::~GeometryNormalMapShader()
{
}

void GeometryNormalMapShader::updateUniforms(GLuint * textures, unsigned nrOf)
{
}

void GeometryNormalMapShader::selfUpdateUniforms(Node * node)
{
	GLuint wmLoc = glGetUniformLocation(this->getID(), "wm");
	if (wmLoc != -1)
	{
		glUniformMatrix4fv(wmLoc, 1, GL_FALSE, &(node->getChainTransform().getMatrix()*node->getLocalTransform().getMatrix())[0][0]);
	}
}
