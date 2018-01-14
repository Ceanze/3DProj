#include "TestShader.h"

#include <glew.h>
#include <gtc\matrix_transform.hpp>

#include "../Core/Config.h"

TestShader::TestShader() : ShaderProgram({ "Phong/Phong.vs", GL_VERTEX_SHADER }, { "Phong/TestShader.fs", GL_FRAGMENT_SHADER })
{
	this->time = 0.0f;
}

TestShader::~TestShader()
{
}

float & TestShader::getTime()
{
	return this->time;
}

void TestShader::selfUpdateUniforms(Node * node)
{
	GLuint mwLoc = glGetUniformLocation(this->getID(), "mw");
	if (mwLoc != -1)
	{
		glUniformMatrix4fv(mwLoc, 1, GL_FALSE, &(node->getChainTransform().getMatrix()*node->getLocalTransform().getMatrix())[0][0]);
	}

	GLuint timeLoc = glGetUniformLocation(this->getID(), "time");
	if (timeLoc != -1)
	{
		glUniform1f(timeLoc, this->time);
	}
}

