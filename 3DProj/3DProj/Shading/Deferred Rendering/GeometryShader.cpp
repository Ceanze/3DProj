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
	GLint wmLoc = glGetUniformLocation(this->getID(), "wm");
	if (wmLoc != -1)
	{
		glUniformMatrix4fv(wmLoc, 1, GL_FALSE, &(node->getChainTransform().getMatrix()*node->getLocalTransform().getMatrix())[0][0]);
	}

	GLint camPosLoc = glGetUniformLocation(this->getID(), "camPos");
	if (camPosLoc != -1)
	{
		glUniform3fv(camPosLoc, 1, &(getCamera()->getPosition())[0]);
	}
	else
		Error::printError("Could not find 'camPos' in geometryDR!");
}
