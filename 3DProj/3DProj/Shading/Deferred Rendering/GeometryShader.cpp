#include "GeometryShader.h"

GeometryShader::GeometryShader() : ShaderProgram({ "Deferred Rendering/GeometryDR.vs", GL_VERTEX_SHADER }, { "Deferred Rendering/GeometryDR.fs", GL_FRAGMENT_SHADER })
{
}

GeometryShader::~GeometryShader()
{
}

void GeometryShader::updateUniforms(FrameBuffer * buffer)
{
}

void GeometryShader::selfUpdateUniforms(Node * node)
{
	GLuint wmLoc = glGetUniformLocation(this->getID(), "wm");
	if (wmLoc != -1)
	{
		glUniformMatrix4fv(wmLoc, 1, GL_FALSE, &(node->getChainTransform().getMatrix()*node->getLocalTransform().getMatrix())[0][0]);
	}
}
