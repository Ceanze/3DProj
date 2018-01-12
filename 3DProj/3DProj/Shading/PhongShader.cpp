#include "PhongShader.h"

#include <glew.h>
#include <gtc\matrix_transform.hpp>

#include "../Core/Config.h"

PhongShader::PhongShader() : ShaderProgram({ "Phong/Phong.vs", GL_VERTEX_SHADER}, { "Phong/Phong.fs", GL_FRAGMENT_SHADER})
{
	this->proj = glm::perspective(glm::pi<float>() / 4.0f, (float)DEFAULT_WIDTH/DEFAULT_HEIGHT, 0.1f, 100.0f);
	this->view = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

PhongShader::~PhongShader()
{
}

void PhongShader::updateUniforms(Node* node)
{
	GLuint camLoc = glGetUniformLocation(this->getID(), "camera");
	if (camLoc != -1)
	{
		glUniformMatrix4fv(camLoc, 1, GL_FALSE, &(this->proj*this->view)[0][0]);
	}

	GLuint mwLoc = glGetUniformLocation(this->getID(), "mw");
	if (mwLoc != -1)
	{
		glUniformMatrix4fv(mwLoc, 1, GL_FALSE, &(node->getWorldTransform().getWorldMatrix()*node->getLocalTransform().getWorldMatrix())[0][0]);
	}
}
