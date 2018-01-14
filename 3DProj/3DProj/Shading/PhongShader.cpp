#include "PhongShader.h"

#include <glew.h>
#include <gtc\matrix_transform.hpp>

#include "../Core/Config.h"

PhongShader::PhongShader() : ShaderProgram({ "Phong/Phong.vs", GL_VERTEX_SHADER}, { "Phong/Phong.fs", GL_FRAGMENT_SHADER})
{
}

PhongShader::~PhongShader()
{
}

void PhongShader::selfUpdateUniforms(Node* node)
{
	GLuint mwLoc = glGetUniformLocation(this->getID(), "mw");
	if (mwLoc != -1)
	{
		glUniformMatrix4fv(mwLoc, 1, GL_FALSE, &(node->getChainTransform().getMatrix()*node->getLocalTransform().getMatrix())[0][0]);
	}
}
