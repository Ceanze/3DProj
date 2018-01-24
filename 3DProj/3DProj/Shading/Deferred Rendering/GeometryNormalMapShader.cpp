#include "GeometryNormalMapShader.h"

#include "../../Error.h"

GeometryNormalMapShader::GeometryNormalMapShader() : ShaderProgram({ "Deferred Rendering/GeometryDRNormalMap.vs", GL_VERTEX_SHADER }, { "Deferred Rendering/GeometryDRNormalMap.fs", GL_FRAGMENT_SHADER }, { "Deferred Rendering/GeometryDRNormalMap.gs", GL_GEOMETRY_SHADER })
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
		Error::printError("Could not find 'camPos' in geometryDRNormalMaps!");

	//Error::printError("Campos: " + std::to_string(getCamera()->getPosition().x) + ", " + std::to_string(getCamera()->getPosition().y) + ", " + std::to_string(getCamera()->getPosition().z));
}
